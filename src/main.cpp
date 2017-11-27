#include <iomanip>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "render.hpp"
#include "scene.hpp"

namespace hw4 {
    void print_image(const Image& img, std::ostream& s) {
        for (int y = 0; y < img.size().y; y += 2) {
            for (int x = 0; x < img.size().x; x++) {
                auto pt = img.get_pixel(glm::ivec2(x, y));
                auto pb = y < img.size().y - 1 ? img.get_pixel(glm::ivec2(x, y + 1)) : glm::vec3(0);

                s << "\033[38;2;"
                  << static_cast<int>(pt.r * 255) << ";"
                  << static_cast<int>(pt.g * 255) << ";"
                  << static_cast<int>(pt.b * 255) << "m"
                  << "\033[48;2;"
                  << static_cast<int>(pb.r * 255) << ";"
                  << static_cast<int>(pb.g * 255) << ";"
                  << static_cast<int>(pb.b * 255) << "m";

                s << "▀";
            }

            s << "\033[0m\n";
        }

        s << std::flush;
    }

    void show_scene(const Scene& scene) {
        RayTraceRenderer render(glm::ivec2(160, 120), glm::radians(90.0f), 5, 4);
        Image img = render.render(
            scene,
            glm::mat4()
        );

        print_image(img, std::cout);
    }

    void render_scene(const Scene& scene) {
        RayTraceRenderer render(glm::ivec2(640, 480), glm::radians(90.0f), 5, 4);
        Image img = render.render(
            scene,
            glm::mat4()
        );

        img.save_as_ppm("test.ppm");
    }

    void populate_scene(Scene& scene) {
        auto mat = std::make_shared<Material>(Material::diffuse(
            glm::vec3(0.7, 0.3, 0.3),
            glm::vec3(0.7, 0.3, 0.3),
            glm::vec3(0.5),
            10
        ));
        auto mat2 = std::make_shared<Material>(Material::reflective(
            Material::diffuse(
                glm::vec3(0, 0, 0.1),
                glm::vec3(0, 0, 0.1),
                glm::vec3(1),
                500
            ),
            0.3
        ));
        auto mat3 = std::make_shared<Material>(Material::reflective(
            Material::diffuse(
                glm::vec3(0, 0.01, 0),
                glm::vec3(0, 0.01, 0),
                glm::vec3(1),
                500
            ),
            0.9
        ));

        scene.objects().push_back(
            std::unique_ptr<Object>(new SphereObject(0.5, glm::vec3(0, 0, -2), mat))
        );
        scene.objects().push_back(
            std::unique_ptr<Object>(new SphereObject(0.5, glm::vec3(0, 0, 2), mat2))
        );
        scene.objects().push_back(std::unique_ptr<Object>(new TriMeshObject(
            std::make_shared<TriMesh>(
                std::vector<Vertex> {
                    Vertex { glm::vec3(2, 2, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) },
                    Vertex { glm::vec3(-2, 2, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) },
                    Vertex { glm::vec3(-2, -2, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) },
                    Vertex { glm::vec3(2, -2, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) }
                },
                std::vector<Triangle> {
                    Triangle { 0, 1, 2 },
                    Triangle { 0, 2, 3 }
                }
            ),
            glm::rotate(glm::translate(glm::mat4(), glm::vec3(-1, 0, 4)), glm::radians(-30.0f), glm::vec3(0, 1, 0)),
            mat
        )));
        scene.objects().push_back(std::unique_ptr<Object>(new TriMeshObject(
            std::make_shared<TriMesh>(
                std::vector<Vertex> {
                    Vertex { glm::vec3(2, 2, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) },
                    Vertex { glm::vec3(-2, 2, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) },
                    Vertex { glm::vec3(-2, -2, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) },
                    Vertex { glm::vec3(2, -2, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) }
                },
                std::vector<Triangle> {
                    Triangle { 0, 1, 2 },
                    Triangle { 0, 2, 3 }
                }
            ),
            glm::rotate(glm::translate(glm::mat4(), glm::vec3(1, 0, 4)), glm::radians(30.0f), glm::vec3(0, 1, 0)),
            mat3
        )));

        scene.point_lights().push_back(std::make_unique<PointLight>(
            glm::vec3(0, 2, 1),
            glm::vec3(0.005),
            glm::vec3(0.2),
            glm::vec3(0.5),
            glm::vec3(1, 0, 0.3)
        ));
    }

    extern "C" int main(int argc, char** argv) {
        Scene scene;

        populate_scene(scene);
        scene.regen_bvh();

        show_scene(scene);
        render_scene(scene);

        return 0;
    }
}
