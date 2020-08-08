#include "App.hpp"
#include "Config.hpp"

int main(void)
{
    App const app = App(Config::APP_NAME);
    return 0;
}
