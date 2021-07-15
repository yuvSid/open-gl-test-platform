#pragma once

#include <string>
#include <vector>

const std::string SHADER_VERTEX_FILEPATH( "./shaders/transferMat.vert" );
const std::string SHADER_FRAGMENT_FILEPATH( "./shaders/twoTextures.frag" );
const std::vector<std::string> TEXTURES_FILENAMES{
	"./resources/textures/container.jpg",
	"./resources/textures/awesomeface.png"
};

const short FPS = 30;
