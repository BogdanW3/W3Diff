#include <vector>
#include "BinaryReader.h"

int load_mod(std::ostream* out, std::string& input, bool debug, bool optional_ints)
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

    const uint32_t objects = reader.read<uint32_t>();
    //*out << "Objects: " << objects << '\n';
	for (size_t i = 0; i < objects; i++)
	{
		*out << '\n'
             << "Original ID: " << reader.read_string(4) << '\n'
             << "Modified ID: " << reader.read_string(4) << '\n';

		const uint32_t modifications = reader.read<uint32_t>();
        //*out << "Modifications: " << modifications << '\n'; // Also probably good to do \t an the beginning of mod lines for readability
		for (size_t j = 0; j < modifications; j++)
        {
			*out << "Modification ID: " << reader.read_string(4) << '\n';
			const uint32_t type = reader.read<uint32_t>();
			if (optional_ints)
				*out << "Level variation: " << reader.read<uint32_t>() << '\n'
                << "Data pointer: " << reader.read<uint32_t>() << '\n';

			std::string data;
			switch (type)
			{
				case 0:
					data = std::to_string(reader.read<int>());
					break;
				case 1:
				case 2:
					data = std::to_string(reader.read<float>());
					break;
				case 3:
					data = reader.read_c_string();
					break;
				default:
					*out << "ERROR: Unknown data type " << type << " while loading modification table.";
					return 13;
			}
			*out << "Modification data: " << data << '\n';
			if (debug)
                *out << "DEBUG: Trail: " << reader.read_string(4) << '\n';
            else
                reader.advance(4);
		}
	}
	return 0;
}
