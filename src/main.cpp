#include "main.hpp"

#include <iostream>

#include <glm/glm.hpp>

int main(void)
{
    glm::mat4 x = glm::mat4(1.0f);
    //App app = App(Config::APP_NAME);
    //app.run();
    std::cout << glm::determinant(x) << std::endl;
    return 0;
}
