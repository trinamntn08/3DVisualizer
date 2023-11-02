//#pragma once
//#include <iostream>
//#include <vector>
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//
//class BaseModel {
//public:
//    BaseModel(std::string vao_name, int tex_id, glm::vec3 pos = glm::vec3(0.0f), 
//        glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) :
//        vao_name(vao_name), m_tex_id(tex_id), m_pos(pos), m_rot(rot), scale(scale)
//    {
//    }
//
//    void Update() {
//        // Your update logic here
//    }
//
//    glm::mat4 GetModelMatrix() {
//        glm::mat4 m_model = glm::mat4(1.0f);
//
//        // Translate
//        m_model = glm::translate(m_model, m_pos);
//
//        // Rotate
//        m_model = glm::rotate(m_model, m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
//        m_model = glm::rotate(m_model, m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
//        m_model = glm::rotate(m_model, m_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
//
//        // Scale
//        m_model = glm::scale(m_model, scale);
//
//        return m_model;
//    }
//
//    void Render() {
//        Update();
//        vao.Render();
//    }
//
//private:
//    glm::vec3 m_pos;
//    glm::vec3 m_rot;
//    glm::vec3 scale;
//    std::string vao_name;
//    glm::mat4 m_model;
//    int m_tex_id;
//};
//
//class ExtendedBaseModel : public BaseModel {
//public:
//    ExtendedBaseModel(App& app, std::string vao_name, int tex_id, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
//        : BaseModel(app, vao_name, tex_id, pos, rot, scale) {
//        OnInit();
//    }
//
//    void Update() {
//        texture.Use(0);
//        program.SetUniform("camPos", camera.GetPosition());
//        program.SetUniform("m_view", camera.GetViewMatrix());
//        program.SetUniform("m_model", GetModelMatrix());
//    }
//
//    void UpdateShadow() {
//        shadow_program.SetUniform("m_model", GetModelMatrix());
//    }
//
//    void RenderShadow() {
//        UpdateShadow();
//        shadow_vao.Render();
//    }
//
//    void OnInit() {
//        program.SetUniform("m_view_light", app.GetLight().GetViewMatrix());
//        program.SetUniform("u_resolution", glm::vec2(app.GetWindowSize()));
//        depth_texture = app.GetMesh().GetTexture("depth_texture");
//        program.SetUniform("shadowMap", 1);
//        depth_texture.Use(1);
//        shadow_vao = app.GetMesh().GetVAO("shadow_" + vao_name);
//        shadow_program = shadow_vao.GetProgram();
//        shadow_program.SetUniform("m_proj", camera.GetProjectionMatrix());
//        shadow_program.SetUniform("m_view_light", app.GetLight().GetViewMatrix());
//        shadow_program.SetUniform("m_model", GetModelMatrix());
//        texture = app.GetMesh().GetTexture(tex_id);
//        program.SetUniform("u_texture_0", 0);
//        texture.Use(0);
//        program.SetUniform("m_proj", camera.GetProjectionMatrix());
//        program.SetUniform("m_view", camera.GetViewMatrix());
//        program.SetUniform("m_model", GetModelMatrix());
//        program.SetUniform("light.position", app.GetLight().GetPosition());
//        program.SetUniform("light.Ia", app.GetLight().GetIa());
//        program.SetUniform("light.Id", app.GetLight().GetId());
//        program.SetUniform("light.Is", app.GetLight().GetIs());
//    }
//
//private:
//    Program shadow_program;  // Your Program class
//    VAO shadow_vao;  // Your VAO class
//    Texture depth_texture;  // Your Texture class
//    Texture texture;  // Your Texture class
//};
//
//class Cube : public ExtendedBaseModel {
//public:
//    Cube(App& app, std::string vao_name = "cube", int tex_id = 0, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
//        : ExtendedBaseModel(app, vao_name, tex_id, pos, rot, scale) {
//    }
//};
//
//class MovingCube : public Cube {
//public:
//    MovingCube(App& app, std::string vao_name, int tex_id, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
//        : Cube(app, vao_name, tex_id, pos, rot, scale) {
//    }
//
//    void Update() {
//        SetModelMatrix(GetModelMatrix());
//        Cube::Update();
//    }
//};