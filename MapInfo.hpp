#include <bitset>
#include <glm/vec2.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <vector>

#include "BinaryReader.h"

int load_map(std::ostream* out, std::string& input, bool debug)
{
    std::ifstream fin(input, std::ios_base::binary);
    fin.seekg(0, std::ios::end);
    const size_t fileSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(fileSize);
    fin.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    fin.close();
    BinaryReader reader(buffer);
	int version;
    *out << "Version: " << (version = reader.read<uint32_t>()) << '\n'
         << "Map version: " << reader.read<uint32_t>() << '\n'
         << "Editor version: " << reader.read<uint32_t>() << '\n'
         << "Game version: " << reader.read<uint32_t>() << '.'<< reader.read<uint32_t>() << '.'<< reader.read<uint32_t>() << '.'<< reader.read<uint32_t>() << '\n'
         << "Map name: " << reader.read_c_string() << '\n'
         << "Author name: " << reader.read_c_string() << '\n'
         << "Description: " << reader.read_c_string() << '\n'
         << "Suggested players: " << reader.read_c_string() << '\n'

         << "Camera bottom left: " << glm::to_string(reader.read<glm::vec2>()) << '\n'
         << "Camera top right: " << glm::to_string(reader.read<glm::vec2>()) << '\n'
         << "Camera top left: " << glm::to_string(reader.read<glm::vec2>()) << '\n'
         << "Camera bottom right: " << glm::to_string(reader.read<glm::vec2>()) << '\n'

         << "Camera complements: " << glm::to_string(reader.read<glm::ivec4>()) << '\n'

         << "Playable width: " << reader.read<uint32_t>() << '\n'
         << "Playable height: " << reader.read<uint32_t>() << '\n';

	const int flags = reader.read<uint32_t>();
	*out << "Hide minimap preview: " << std::boolalpha << (bool)(flags & 0x0001) << '\n'
         << "Modified ally properties: " << (bool)(flags & 0x00002) << '\n'
         << "Melee: " << (bool)(flags & 0x00004) << '\n'
         << "Was large: " << (bool)(flags & 0x00008) << '\n'
         << "Partial masked area visibility: " << (bool)(flags & 0x00010) << '\n'
         << "Fixed player settings: " << (bool)(flags & 0x00020) << '\n'
         << "Custom forces: " << (bool)(flags & 0x00040) << '\n'
         << "Custom techtree: " << (bool)(flags & 0x00080) << '\n'
         << "Custom abilities: " << (bool)(flags & 0x00100) << '\n'
         << "Custom upgrades: " << (bool)(flags & 0x00200) << '\n'
         << "Properties menu opened: " << (bool)(flags & 0x00400) << '\n'
         << "Cliff shore waves: " << (bool)(flags & 0x00800) << '\n'
         << "Rolling shore waves: " << (bool)(flags & 0x01000) << '\n'
         << "Terrain fog: " << (bool)(flags & 0x02000) << '\n'
         << "Expansion required: " << (bool)(flags & 0x04000) << '\n'
         << "Item classification: " << (bool)(flags & 0x08000) << '\n'
         << "Water tinting: " << (bool)(flags & 0x10000) << '\n'
         << "Accurate probablility: " << (bool)(flags & 0x20000) << '\n'
         << "Ability skins: " << (bool)(flags & 0x40000) << '\n'
         << "Tileset: " << reader.read_string(1) << '\n'

         << "Loading screen number: " << reader.read<int32_t>() << '\n'
         << "Loading screen model: " << reader.read_c_string() << '\n'
         << "Loading screen text: " << reader.read_c_string() << '\n'
         << "Loading screen title: " << reader.read_c_string() << '\n'
         << "Loading screen subtitle: " << reader.read_c_string() << '\n'

         << "Game data set: " << reader.read<uint32_t>() << '\n'

         << "Prologue screen model: " << reader.read_c_string() << '\n'
         << "Prologue screen text: " << reader.read_c_string() << '\n'
         << "Prologue screen title: " << reader.read_c_string() << '\n'
         << "Prologue screen subtitle: " << reader.read_c_string() << '\n'

         << "Fog style: " << reader.read<uint32_t>() << '\n'
         << "Fog start Z: " << reader.read<float>() << '\n'
         << "Fog end Z: " << reader.read<float>() << '\n'
         << "Fog density: " << reader.read<float>() << '\n'
         << "Fog colour: " << glm::to_string(reader.read<glm::u8vec4>()) << '\n'

         << "Weather ID: " << reader.read<uint32_t>() << '\n'
         << "Custom sound path: " << reader.read_c_string() << '\n'
         << "Custom light tileset: " << reader.read<uint8_t>() << '\n'
         << "Water colour: " << glm::to_string(reader.read<glm::u8vec4>()) << '\n'

         << "Lua: " << (bool)reader.read<uint32_t>() << '\n';

    if (version >= 31)
    {
        const int modes = reader.read<uint32_t>();
    *out << "Supports SD: " << (bool)(modes & 0x0001) << '\n'
         << "Supports HD: " << (bool)(modes & 0x0002) << '\n'
         << "TFT game data: " << (bool)(reader.read<uint32_t>() - 1) << "\n\n";
    }

	uint32_t players = reader.read<uint32_t>();
	//*out << "Number of players: " << players << "\n\n";
	for (uint32_t i = 0; i < players; i++)
    {
        *out << "Number: " << reader.read<uint32_t>() << '\n'
             << "Type: " << reader.read<uint32_t>() << '\n'
             << "Race: " << reader.read<uint32_t>() << '\n'
             << "Fixed start position: " << (bool)reader.read<uint32_t>() << '\n'
             << "Name: " << reader.read_c_string() << '\n'
             << "Starting position: " << glm::to_string(reader.read<glm::vec2>()) << '\n'
             << "Ally low priorities: " << std::bitset<32>(reader.read<uint32_t>()) << '\n'
             << "Ally high priorities: " << std::bitset<32>(reader.read<uint32_t>()) << '\n';
		if (version >= 31)
        *out << "Enemy low priorities: " << std::bitset<32>(reader.read<uint32_t>()) << '\n'
             << "Enemy high priorities: " << std::bitset<32>(reader.read<uint32_t>()) << '\n';
	}

	uint32_t forces = reader.read<uint32_t>();
	//*out << "Number of buttons: " << buttons << "\n\n";
	for (uint32_t i = 0; i < forces; i++)
    {
		const uint32_t force_flags = reader.read<uint32_t>();
		*out << "Allied: " << (bool)(force_flags & 0x0001) << '\n'
             << "Allied victory: " << (bool)(force_flags & 0x00002) << '\n'
             << "Share vision: " << (bool)(force_flags & 0x00004) << '\n'
             << "Share unit control: " << (bool)(force_flags & 0x00008) << '\n'
             << "Share advanced unit control: " << (bool)(force_flags & 0x00010) << '\n'
             << "Players: " << std::bitset<32>(reader.read<uint32_t>()) << '\n'
             << "Name: " << reader.read_c_string() << "\n\n";
	}

	uint32_t upgrades = reader.read<uint32_t>();
	//*out << "Number of upgrades: " << upgrades << "\n\n";
	for (uint32_t i = 0; i < upgrades; i++)
    {
        *out << "Upgrade Players: " << std::bitset<32>(reader.read<uint32_t>()) << '\n'
             << "Upgrade ID: " << reader.read_string(4) << '\n'
             << "Affected level: " << (reader.read<uint32_t>() + 1) << '\n'
             << "Availability: " << reader.read<uint32_t>() << "\n\n";
	}

	uint32_t tech = reader.read<uint32_t>();
	//*out << "Number of tech modifications: " << tech << "\n\n";
	for (uint32_t i = 0; i < tech; i++)
    {
        *out << "Tech Unavailable for Players: " << std::bitset<32>(reader.read<uint32_t>()) << '\n'
             << "Tech ID: " << reader.read_string(4) << "\n\n";
	}

	uint32_t unit_groups = reader.read<uint32_t>();
	//*out << "Number of unit groups: " << unit_groups << "\n\n";
	for (uint32_t i = 0; i < unit_groups; i++)
    {
		*out << "\nGroup number: " << reader.read<uint32_t>() << '\n'
             << "Group name: " << reader.read_c_string() << '\n';
		uint32_t positions = reader.read<uint32_t>();
		for (uint32_t j = 0; j < positions; j++)
        {
            *out << "Table type: " << reader.read<uint32_t>() << '\n';
        }
        uint32_t lines = reader.read<uint32_t>();
		for (uint32_t j = 0; j < lines; j++)
        {
            *out << "Line chance: " << reader.read<uint32_t>() << '\n'
                 << "Unit IDs: ";
            for (uint32_t k = 0; k < positions; k++)
                *out << reader.read_string(4) << ' ';
            *out << '\n';
        }
		*out << '\n';
	}

	uint32_t item_groups = reader.read<uint32_t>();
	//*out << "Number of item groups: " << item_groups << "\n\n";
	for (uint32_t i = 0; i < item_groups; i++)
    {
		*out << "\nGroup number: " << reader.read<uint32_t>() << '\n'
             << "Group name: " << reader.read_c_string() << '\n';
		uint32_t positions = reader.read<uint32_t>();
        uint32_t lines = reader.read<uint32_t>();
		for (uint32_t j = 0; j < lines; j++)
        {
            *out << "Line chance: " << reader.read<uint32_t>() << '\n';
            for (uint32_t k = 0; k < positions; k++)
                *out << "Item ID: " << reader.read_string(4) << ' ';
            *out << '\n';
        }
	}
	return 0;
}
