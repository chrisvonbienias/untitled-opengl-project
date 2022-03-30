#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

const unsigned int WIDTH = 640;
const unsigned int HEIGHT = 480;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // sync with monitor's refresh rate

    /* Initialize GLEW */
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW error!" << std::endl;

    /* Print out OpenGL version */
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    /* Blending mode */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    /* Basic triangle */ 
    float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f,
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f,
        100.0f, 200.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    /* Index buffer object */
    IndexBuffer ib(indices, 6);

    /* Projection matirx */
    /* First 4 variables need to represent your aspect ratio */
    /* In this case:  |-2.0 - 2.0| / |-1.5 - 1.5| = 4/3 */
    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);
    glm::vec4 vertex_pos(100.0f, 100.0f, 0.0f, 1.0f);
    glm::vec4 result = proj * vertex_pos;

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
    shader.SetUniform4f("u_MVP", proj);

    Texture texture("res/textures/dupa.png");
    texture.Bind(0);

    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    shader.UnBind();

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        shader.SetUniform1i("u_Texture", 0);

        renderer.Draw(va, ib, shader);


        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Clean-up */
    glfwTerminate();
    
    return 0;
}