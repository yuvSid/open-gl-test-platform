#pragma once

#include <string>
#include <vector>

const std::string SHADER_VERTEX_FILEPATH( "./resources/shaders/texture_normals.vert" );
const std::string SHADER_FRAGMENT_FILEPATH( "./resources/shaders/material_texture.frag" );
const std::string SHADER_VERT_COORDS_TRANSFORM( "./resources/shaders/coords_with_transform.vert" );
const std::string SHADER_FRAG_ONE_COLOR( "./resources/shaders/light_caster.frag" );

const std::vector<std::string> TEXTURES_FILENAMES{
	"./resources/textures/container2.png",
	"./resources/textures/container2_specular.png"
};

const short FPS = 30;
