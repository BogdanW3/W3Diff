#include <vector>
#include "BinaryReader.h"

int load_region(std::ostream* out, std::string& input, bool debug)
{
    std::ifstream fin(input, std::ios_base::binary);
    fin.seekg(0, std::ios::end);
    const size_t fileSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(fileSize);
    fin.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    fin.close();
    BinaryReader reader(buffer);

	*out << "Version: " << reader.read<uint32_t>() << '\n';

	size_t regions = reader.read<uint32_t>();
	//*out << "Number of cameras: " << cameras << "\n\n";
	for (size_t i = 0; i < regions; i++)
    {
        try
        {
            *out << "Left: " << reader.read<float>() << '\n'
                 << "Bottom: " << reader.read<float>() << '\n'
                 << "Right: " << reader.read<float>() << '\n'
                 << "Top: " << reader.read<float>() << '\n'
                 << "Name: " << reader.read_c_string() << '\n'
                 << "Creation number: " << reader.read<int>() << '\n'
                 << "Weather ID: " << reader.read_string(4) << '\n'
                 << "Ambient ID: " << reader.read_c_string() << '\n'
                 << "Colour: " << glm::to_string(reader.read<glm::u8vec3>()) << '\n';

            if (debug)
                *out << "DEBUG: Last byte: " << reader.read<uint8_t>();
            else
                reader.advance(1);
            *out << "\n\n";
        }
        catch (std::out_of_range& ex)
        {
            std::cerr << ex.what();
            return 13;
        }
	}
	return 0;
}
