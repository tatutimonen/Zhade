#include "Main.hpp"

int main(void)
{
    App const app = App(Config::APP_NAME);
    app.run();
    return 0;
}
