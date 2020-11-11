#include <glm/vec2.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <vector>

#include "BinaryReader.h"

int load_terrain(std::ostream* out, std::string& input, bool debug)
{
    std::ifstream fin(input, std::ios_base::binary);
    fin.seekg(0, std::ios::end);
    const size_t fileSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(fileSize);
    fin.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    fin.close();
    BinaryReader reader(buffer);

	const std::string magic_number = reader.read_string(4);
	if (magic_number != "W3E!") {
		std::cerr << "Invalid war3map.w3e file: Magic number is not W3E!";
		return 13;
	}
	// Version
	*out << "Version: " << reader.read<uint32_t>() << '\n'

	<< "Tileset: " << reader.read<char>() << '\n'
	<< "Custom Tileset: " << std::boolalpha << (bool)reader.read<uint32_t>() << '\n';

	const uint32_t tileset_textures = reader.read<uint32_t>();
	*out << "Tileset textures: ";
	for (size_t i = 0; i < tileset_textures; i++) {
		*out << reader.read_string(4) << ' ';
	}

	const int cliffset_textures = reader.read<uint32_t>();
	*out << "\nCliffset textures: ";
	for (int i = 0; i < cliffset_textures; i++) {
		*out << reader.read_string(4) << ' ';
	}

	uint32_t width = reader.read<uint32_t>();
	uint32_t height = reader.read<uint32_t>();
	*out << "\nWidth: " << width << '\n'
	<< "Height: " << height << '\n'

	<< "Offset: " << glm::to_string(reader.read<glm::vec2>()) << "\n\n";

	if (debug) //Terrain changes are not readable
        for (uint32_t j = 0; j < height; j++)
            for (uint32_t i = 0; i < width; i++) {

                *out << "Height: " << reader.read<uint16_t>() << '\n';

                const uint16_t water_and_edge = reader.read<uint16_t>();
                *out << "Water height: " << (water_and_edge & 0x3FFF) << '\n'
                     << "Is map edge: " << (bool)(water_and_edge & 0x4000) << '\n';

                const uint8_t texture_and_flags = reader.read<uint8_t>();
                *out << "ground texture: " << (texture_and_flags & 0b00001111) << '\n'

                << "Has ramp: "     << (bool)(texture_and_flags & 0b00010000) << '\n'
                << "Has blight: "   << (bool)(texture_and_flags & 0b00100000) << '\n'
                << "Has water: "    << (bool)(texture_and_flags & 0b01000000) << '\n'
                << "Has boundary: " << (bool)(texture_and_flags & 0b10000000) << '\n';

                const uint8_t variation = reader.read<uint8_t>();
                *out << "Ground variation: " << (variation & 0b00011111) << '\n'
                << "Cliff variation: " << ((variation & 0b11100000) >> 5) << '\n';

                const uint8_t misc = reader.read<uint8_t>();
                *out << "Cliff texture: " << ((misc & 0b11110000) >> 4) << '\n'
                << "Layer height: " << (misc & 0b00001111) << "\n\n";
            }
	return 0;
}
