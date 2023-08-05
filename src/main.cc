#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <dpp/dpp.h> // include the D++ library

int main()
{
    unsigned int num_cores(std::thread::hardware_concurrency());
    // create a bot object with your Discord bot token
    dpp::cluster dominic("MTA5MjI0NzExMzg4ODQ0ODUyMg.GyoD2C.nvCoqDhqtjJwmj5_d5-2UC91bwMN4046WBLqmc", dpp::i_default_intents, 10);
    dominic.on_log(dpp::utility::cout_logger());

    dominic.on_slashcommand([&dominic](const dpp::slashcommand_t &event)
                            {
         if (event.command.get_command_name() == "imagine") {
            std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
            std::string negative_prompt ;
            int64_t steps;
            try {
            negative_prompt = "NSFW, sex, porn "+std::get<std::string>(event.get_parameter("negative_prompt"));
            steps = std::get<int64_t>(event.get_parameter("steps"));
            } catch (const std::bad_variant_access&) {
                negative_prompt = "NSFW, sex, porn";
                steps = 20;
            }
            std::random_device rd;  // Will be used to obtain a seed for the random number engine
            std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
            std::uniform_int_distribution<> distrib(0, 10000000);

            int random_number = distrib(gen);
            dpp::json inference_payload = {
            {"prompt", prompt},
            {"neg_prompt", negative_prompt},
            {"unet_model", "meinamix"},
            {"seed", random_number},
            {"steps", steps}
        };
            std::string result_url;
            // HTTPS
            httplib::Client cli("https://sd-inference.jan.ai");
            httplib::Headers headers = {
                    {"Content-Type", "application/json"},
                };
            auto res = cli.Post("/sd_inference", headers, inference_payload.dump(), "application/json");
                if (res && res->status == 200) {
                    result_url = dpp::json::parse(res->body)["url"];
                } else {
                    
                    event.reply("Unkown bug happen, cannot generate image");
                    return;
                }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            dominic.log(dpp::loglevel::ll_info, "Finished processing an image");
            event.reply(result_url);
            return;
        } });

    dominic.on_ready([&dominic](const dpp::ready_t &event)
                     {
        dominic.log(dpp::loglevel::ll_info, "Bot is up with shard id: " + std::to_string(event.shard_id));
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand imagine("imagine","Type your description of the image you want",dominic.me.id);
            dpp::command_option prompt_option(dpp::co_string,"prompt","Description of your image",true);
            dpp::command_option negative_prompt_option(dpp::co_string,"negative_prompt","Things you do not want to see in the image",false);
            dpp::command_option steps_option(dpp::co_integer,"steps","numbers of diffusion steps",false);
            imagine.add_option(prompt_option).add_option(negative_prompt_option).add_option(steps_option);
            dominic.global_command_create(imagine);
        } });
    // start the bot
    dominic.start(dpp::st_wait);
    return 0;
}
