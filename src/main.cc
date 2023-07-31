#include <dpp/dpp.h> // include the D++ library
#include <iostream>

int main()
{
    // create a bot object with your Discord bot token
    dpp::cluster dominic("MTA5MjI0NzExMzg4ODQ0ODUyMg.GyoD2C.nvCoqDhqtjJwmj5_d5-2UC91bwMN4046WBLqmc");
    dominic.on_log(dpp::utility::cout_logger());
    dpp::message msg(1093368776910716979, "Bot is up!");
    dominic.message_create(msg);
    dominic.on_slashcommand([&dominic](const dpp::slashcommand_t &event)
                            {
         if (event.command.get_command_name() == "imagine") {
            std::string prompt = std::get<std::string>(event.get_parameter("prompt"));
            std::string negative_prompt ;
            int steps;
            try {
            negative_prompt = std::get<std::string>(event.get_parameter("negative_prompt"));
            steps = std::get<int64_t>(event.get_parameter("steps"));
            } catch (const std::bad_variant_access&) {
                negative_prompt = "asdfasdf";
                steps = 20;
            }

            dominic.log(dpp::loglevel::ll_info, "Finished processing an image");
            dpp::json inference_payload = {
            {"prompt", prompt},
            {"neg_prompt", negative_prompt},
            {"unet_model", "openjourney"},
            {"seed", 37232013},
            {"steps", steps}
        };

                std::string payload = R"(
                    {
                    "prompt": "a",
                    "neg_prompt": "cat",
                    "unet_model": "openjourney",
                    "seed": 12312312,
                    "steps": 20
                    }
                    )";
            dominic.request(
            "https://sd-inference.jan.ai/sd_inference", dpp::m_post, [&event](const dpp::http_request_completion_t &cc) {
                // This callback is called when the HTTP request completes. See documentation of
                // dpp::http_request_completion_t for information on the fields in the parameter.
                            std::cout<< cc.status;
                            event.reply();
            },
            payload  // convert the json object to a string
            ,"text/plain",{{"Content-Type", "application/json"}});

            
        } });

    dominic.on_ready([&dominic](const dpp::ready_t &event)
                     {
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand imagine("imagine","Type your description of the image you want",dominic.me.id);
            dpp::command_option prompt_option(dpp::co_string,"prompt","Description of your image",true);
            dpp::command_option negative_prompt_option(dpp::co_string,"negative_prompt","Things you do not want to see in the image",false);
            dpp::command_option steps_option(dpp::co_integer,"steps","numbers of diffusion steps",false);
            imagine.add_option(prompt_option).add_option(negative_prompt_option);
            dominic.global_command_create(imagine);
        } });
    // start the bot
    dominic.start(dpp::st_wait);
    return 0;
}