#include <iostream>
using std::cout;
using std::endl;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int main(void)
{
    auto m1 = glm::mat4(2.f);
    auto v1 = glm::vec4(1.f, 2.f, 3.f, 1.f);
    auto res = m1 * v1;
    cout << res.x << endl;
    return 0;
}