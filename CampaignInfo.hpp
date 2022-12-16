#include <glm/vec2.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <vector>

#include "BinaryReader.h"

int load_campaign(std::ostream* out, std::string& input, bool debug)
{
    std::ifstream fin(input, std::ios_base::binary);
    fin.seekg(0, std::ios::end);
    const size_t fileSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(fileSize);
    fin.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    fin.close();
    BinaryReader reader(buffer);
    *out << "Version: " << reader.read<uint32_t>() << '\n'
         << "Campaign version: " << reader.read<uint32_t>() << '\n'
         << "Editor version: " << reader.read<uint32_t>() << '\n'
         << "Campaign name: " << reader.read_c_string() << '\n'
         << "Difficulty: " << reader.read_c_string() << '\n'
         << "Author name: " << reader.read_c_string() << '\n'
         << "Description: " << reader.read_c_string() << '\n';

	const int flags = reader.read<uint32_t>();
	*out << "Variable difficulty: " << std::boolalpha << (flags & 0x0001) << '\n'
         << "Expansion required: " << (bool)(flags & 0x0002) << '\n'

         << "Background screen number: " << reader.read<uint32_t>() << '\n'
         << "Background screen model: " << reader.read_c_string() << '\n'
         << "Campaign image path: " << reader.read_c_string() << '\n'

         << "Custom sound number: " << reader.read<uint32_t>() << '\n'
         << "Custom sound path: " << reader.read_c_string() << '\n'

         << "Fog style: " << reader.read<uint32_t>() << '\n'
         << "Fog start Z: " << reader.read<float>() << '\n'
         << "Fog end Z: " << reader.read<float>() << '\n'
         << "Fog density: " << reader.read<float>() << '\n'
         << "Fog colour: " << glm::to_string(reader.read<glm::u8vec4>()) << '\n'

         << "UI race: " << reader.read<uint32_t>() << "\n\n";

	uint32_t buttons = reader.read<uint32_t>();
	//*out << "Number of buttons: " << buttons << "\n\n";
	for (uint32_t i = 0; i < buttons; i++)
    {
        *out << "Visible: " << (bool)reader.read<uint32_t>() << '\n'
             << "Chapter name: " << reader.read_c_string() << '\n'
             << "Button name: " << reader.read_c_string() << '\n'
             << "Map path: " << reader.read_c_string() << "\n\n";
	}
	uint32_t maps = reader.read<uint32_t>();
	//*out << "Number of maps: " << maps << "\n\n";
	for (uint32_t i = 0; i < maps; i++)
    {
        if (debug)
            *out << "DEBUG: Unknown: " << reader.read<uint8_t>() << '\n';
        else
            reader.read<uint8_t>();
		*out << "Map path: " << reader.read_c_string() << "\n\n";
	}
	return 0;
}
