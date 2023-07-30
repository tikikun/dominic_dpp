#include <dpp/dpp.h> // include the D++ library
#include <iostream>

int main()
{
    // create a bot object with your Discord bot token
    dpp::cluster my_bot("MTA5MjI0NzExMzg4ODQ0ODUyMg.GyoD2C.nvCoqDhqtjJwmj5_d5-2UC91bwMN4046WBLqmc");
    dpp::message msg(1093368776910716979, "Are you good?");
    my_bot.message_create(msg);

    my_bot.on_slashcommand([](const dpp::slashcommand_t &event)
                           {
         if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        } });

    my_bot.on_slashcommand([](const dpp::slashcommand_t &event)
                           {
         if (event.command.get_command_name() == "alan_test") {
            std::string input_testing = std::get<std::string>(event.get_parameter("testing"));
            event.reply(input_testing);
        } });

    my_bot.on_ready([&my_bot](const dpp::ready_t &event)
                    {
        if (dpp::run_once<struct register_bot_commands>()) {
            my_bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", my_bot.me.id));
            dpp::slashcommand alan_test("alan_test", "Ping pong!", my_bot.me.id);
            alan_test.add_option(
                dpp::command_option(dpp::co_string,"testing","The testing to fill here",true)
            );
            my_bot.global_command_create(alan_test);
        } });
    // start the bot
    my_bot.start(dpp::st_wait);
    return 0;
}