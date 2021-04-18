#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

#include "Camera.hpp"
#include "CampaignInfo.hpp"
#include "MapInfo.hpp"
#include "Mod.hpp"
#include "Region.hpp"
#include "Terrain.hpp"

int to_text(std::string& input, std::string& output, bool debug)
{
    std::ostream *out;
    if (output.compare("-1"))
    {
        out = new std::ofstream(output);
    } else
        out = &std::cout;
    //*out << "test\n";
    int errorcode = 0;
    std::string ext = &input[input.length()-3];
    if (!(ext.compare("w3b") && ext.compare("w3h") && ext.compare("w3t") && ext.compare("w3u")))
    {
        errorcode = load_mod(out, input, debug, false);
    } else if (!(ext.compare("w3a") && ext.compare("w3d") && ext.compare("w3q")))
    {
        errorcode = load_mod(out, input, debug, true);
    } else if (!ext.compare("w3c"))
    {
        errorcode = load_camera(out, input, debug);
    } else if (!ext.compare("w3e"))
    {
        errorcode = load_terrain(out, input, debug);
    } else if (!ext.compare("w3f"))
    {
        errorcode = load_campaign(out, input, debug);
    } else if (!ext.compare("w3i"))
    {
        errorcode = load_map(out, input, debug);
    } else if (!ext.compare("w3r"))
    {
        errorcode = load_region(out, input, debug);
    } //I am NOT doing w3s until a better format is made, contributions are welcome
    return errorcode;
}
