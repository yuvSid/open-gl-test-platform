#pragma once

#include <string>
#include <vector>

const std::string SHADER_VERTEX_FILEPATH( "./shaders/texture_normals.vert" );
const std::string SHADER_FRAGMENT_FILEPATH( "./shaders/material_texture.frag" );
const std::string SHADER_VERT_COORDS_TRANSFORM( "./shaders/coords_with_transform.vert" );
const std::string SHADER_FRAG_ONE_COLOR( "./shaders/light_caster.frag" );

const std::vector<std::string> TEXTURES_FILENAMES{
	"./resources/textures/container2.png",
	"./resources/textures/container2_specular.png"
};

const short FPS = 30;
