#include <vector>
#include "BinaryReader.h"

int load_camera(std::ostream* out, std::string& input, bool debug)
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

	size_t cameras = reader.read<uint32_t>();
	//*out << "Number of cameras: " << cameras << "\n\n";
	for (size_t i = 0; i < cameras; i++)
    {
        try
        {
        *out << "Target X: " << reader.read<float>() << '\n'
                << "Target Y: " << reader.read<float>() << '\n'
                << "Z offset: " << reader.read<float>() << '\n'
                << "Rotation: " << reader.read<float>() << '\n'
                << "Angle of attack: " << reader.read<float>() << '\n'
                << "Distance: " << reader.read<float>() << '\n'
                << "Roll: " << reader.read<float>() << '\n'
                << "Field of view: " << reader.read<float>() << '\n'
                << "Far Z: " << reader.read<float>() << '\n'
                << "Near Z: " << reader.read<float>() << '\n'
                << "Local Pitch: " << reader.read<float>() << '\n'
                << "Local Yaw: " << reader.read<float>() << '\n'
                << "Local Roll: " << reader.read<float>() << '\n'
                << "Name: " << reader.read_c_string() << "\n\n";
        }
        catch (std::out_of_range& ex)
        {
            std::cerr << ex.what();
            return 13;
        }
	}
	return 0;
}
