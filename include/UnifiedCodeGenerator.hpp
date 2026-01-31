#pragma once

#include "SimpleJSON.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <random>

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * Unified Code Generator for Complete CODESYS + Webots Automation
 * 
 * Generates all required files from a single JSON schema:
 * 1. PLCopenXML (CODESYS import)
 * 2. C++ types
 * 3. C++ typed client  
 * 4. Webots controller
 * 5. Usage README
 */

class UnifiedCodeGenerator {
public:
    struct Variable {
        std::string name;
        std::string type;
        std::string direction;
        int size;
        
        Variable() : size(0) {}
    };

    std::string memoryName;
    std::vector<Variable> variables;
    
    // UUIDs for PLCopenXML
    std::string uuidMain, uuidToCPP, uuidFromCPP;

    void loadSchema(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file) throw std::runtime_error("Cannot open schema file: " + filepath);

        std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        auto root = SimpleJSON::parse(json);

        memoryName = root.getString("name");
        if (memoryName.empty()) throw std::runtime_error("Schema missing 'name' field");

        auto vars = root.getArray("variables");
        for (const auto& v : vars) {
            Variable var;
            var.name = v.getString("name");
            var.type = v.getString("type");
            var.direction = v.getString("direction");
            
            try { var.size = v.getInt("size"); }
            catch(...) { var.size = 0; }
            
            if (var.name.empty() || var.type.empty() || var.direction.empty()) {
                throw std::runtime_error("Variable missing required fields");
            }
            
            if (var.direction != "toCPP" && var.direction != "fromCPP") {
                throw std::runtime_error("Invalid direction: " + var.direction);
            }
            
            if (var.type == "STRING" && var.size == 0) {
                throw std::runtime_error("STRING type requires 'size' field: " + var.name);
            }
            
            variables.push_back(var);
        }
        
        // Generate UUIDs once for consistency
        uuidMain = generateUUID();
        uuidToCPP = generateUUID();
        uuidFromCPP = generateUUID();
    }

    std::string generateUUID() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 15);
        static std::uniform_int_distribution<> dis2(8, 11);

        std::stringstream ss;
        ss << std::hex;
        for (int i = 0; i < 8; i++) ss << dis(gen);
        ss << "-";
        for (int i = 0; i < 4; i++) ss << dis(gen);
        ss << "-4";
        for (int i = 0; i < 3; i++) ss << dis(gen);
        ss << "-";
        ss << dis2(gen);
        for (int i = 0; i < 3; i++) ss << dis(gen);
        ss << "-";
        for (int i = 0; i < 12; i++) ss << dis(gen);
        return ss.str();
    }

    std::string getCurrentTimestamp() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S.0000000");
        return ss.str();
    }

    // Create directory recursively (Windows)
    void createDirectories(const std::string& path) {
        #ifdef _WIN32
        CreateDirectoryA(path.c_str(), NULL);
        #else
        mkdir(path.c_str(), 0755);
        #endif
    }

    // Simple file copy
    void copyFile(const std::string& src, const std::string& dst) {
        std::ifstream srcFile(src, std::ios::binary);
        std::ofstream dstFile(dst, std::ios::binary);
        if (srcFile && dstFile) {
            dstFile << srcFile.rdbuf();
        } else {
            std::cerr << "Warning: Could not copy " << src << " to " << dst << std::endl;
        }
    }


    std::string typeToXML(const Variable& var) {
        if (var.type == "STRING") return "<string />";
        return "<" + var.type + " />";
    }

    void generateVariable(std::ostream& out, const Variable& var, int indent = 12) {
        std::string spaces(indent, ' ');
        out << spaces << "<variable name=\"" << var.name << "\">\n";
        out << spaces << "  <type>\n";
        out << spaces << "    " << typeToXML(var) << "\n";
        out << spaces << "  </type>\n";
        out << spaces << "</variable>\n";
    }

    void generateStruct(std::ostream& out, const std::string& structName, 
                        const std::vector<Variable>& fields, const std::string& uuid) {
        out << "      <dataType name=\"" << structName << "\">\n";
        out << "        <baseType>\n";
        out << "          <struct>\n";
        
        if (fields.empty()) {
            out << "            <variable name=\"dummy\">\n";
            out << "              <type>\n";
            out << "                <BYTE />\n";
            out << "              </type>\n";
            out << "              <documentation>\n";
            out << "                <xhtml xmlns=\"http://www.w3.org/1999/xhtml\"> Ensure non-empty struct </xhtml>\n";
            out << "              </documentation>\n";
            out << "            </variable>\n";
        } else {
            for (const auto& var : fields) {
                generateVariable(out, var);
            }
        }
        
        out << "          </struct>\n";
        out << "        </baseType>\n";
        out << "        <addData>\n";
        out << "          <data name=\"http://www.3s-software.com/plcopenxml/objectid\" handleUnknown=\"discard\">\n";
        out << "            <ObjectId>" << uuid << "</ObjectId>\n";
        out << "          </data>\n";
        out << "        </addData>\n";
        out << "      </dataType>\n";
    }

    // 1. Generate PLCopenXML
    void generatePLCopenXML(const std::string& outputPath) {
        std::ofstream out(outputPath);
        std::string timestamp = getCurrentTimestamp();
        
        std::vector<Variable> toCPPVars, fromCPPVars;
        for (const auto& v : variables) {
            if (v.direction == "toCPP") toCPPVars.push_back(v);
            else fromCPPVars.push_back(v);
        }
        
        out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
        out << "<project xmlns=\"http://www.plcopen.org/xml/tc6_0200\">\n";
        out << "  <fileHeader companyName=\"\" productName=\"CODESYS\" productVersion=\"CODESYS V3.5 SP20 Patch 3\" creationDateTime=\"" << timestamp << "\" />\n";
        out << "  <contentHeader name=\"Generated_" << memoryName << "\" modificationDateTime=\"" << timestamp << "\">\n";
        out << "    <coordinateInfo>\n";
        out << "      <fbd><scaling x=\"1\" y=\"1\" /></fbd>\n";
        out << "      <ld><scaling x=\"1\" y=\"1\" /></ld>\n";
        out << "      <sfc><scaling x=\"1\" y=\"1\" /></sfc>\n";
        out << "    </coordinateInfo>\n";
        out << "    <addData>\n";
        out << "      <data name=\"http://www.3s-software.com/plcopenxml/projectinformation\" handleUnknown=\"implementation\">\n";
        out << "        <ProjectInformation />\n";
        out << "      </data>\n";
        out << "    </addData>\n";
        out << "  </contentHeader>\n";
        out << "  <types>\n";
        out << "    <dataTypes>\n";
        
        // Main struct
        out << "      <dataType name=\"ST_" << memoryName << "\">\n";
        out << "        <baseType>\n";
        out << "          <struct>\n";
        out << "            <variable name=\"ToCPP\"><type><derived name=\"ST_ToCPP\" /></type></variable>\n";
        out << "            <variable name=\"FromCPP\"><type><derived name=\"ST_FromCPP\" /></type></variable>\n";
        out << "            <variable name=\"LifePLC\"><type><DINT /></type><documentation><xhtml xmlns=\"http://www.w3.org/1999/xhtml\"> PLC heartbeat counter </xhtml></documentation></variable>\n";
        out << "            <variable name=\"LifeCPP\"><type><DINT /></type><documentation><xhtml xmlns=\"http://www.w3.org/1999/xhtml\"> C++ heartbeat counter </xhtml></documentation></variable>\n";
        out << "          </struct>\n";
        out << "        </baseType>\n";
        out << "        <addData><data name=\"http://www.3s-software.com/plcopenxml/objectid\" handleUnknown=\"discard\"><ObjectId>" << uuidMain << "</ObjectId></data></addData>\n";
        out << "      </dataType>\n";
        
        generateStruct(out, "ST_ToCPP", toCPPVars, uuidToCPP);
        generateStruct(out, "ST_FromCPP", fromCPPVars, uuidFromCPP);
        
        out << "    </dataTypes>\n";
        out << "    <pous />\n";
        out << "  </types>\n";
        out << "  <instances><configurations /></instances>\n";
        out << "  <addData>\n";
        out << "    <data name=\"http://www.3s-software.com/plcopenxml/projectstructure\" handleUnknown=\"discard\">\n";
        out << "      <ProjectStructure>\n";
        out << "        <Object Name=\"ST_" << memoryName << "\" ObjectId=\"" << uuidMain << "\" />\n";
        out << "        <Object Name=\"ST_ToCPP\" ObjectId=\"" << uuidToCPP << "\" />\n";
        out << "        <Object Name=\"ST_FromCPP\" ObjectId=\"" << uuidFromCPP << "\" />\n";
        out << "      </ProjectStructure>\n";
        out << "    </data>\n";
        out << "  </addData>\n";
        out << "</project>\n";
    }

    // 2. Generate C++ Types
    void generateCppTypes(const std::string& outputPath) {
        std::ofstream out(outputPath);
        
        out << "#pragma once\n";
        out << "#include \"CodesysTypes.hpp\"\n";
        out << "#include <cstring>\n\n";
        out << "// Auto-generated from schema: " << memoryName << "\n\n";
        out << "#pragma pack(push, 1)\n\n";

        out << "struct ST_ToCPP {\n";
        for (const auto& v : variables) {
            if (v.direction == "toCPP") {
                out << "    ";
                if (v.type == "STRING") {
                    out << "char " << v.name << "[" << (v.size + 1) << "];  // STRING[" << v.size << "] + null\n";
                } else {
                    out << v.type << " " << v.name << ";\n";
                }
            }
        }
        out << "};\n\n";

        out << "struct ST_FromCPP {\n";
        bool hasFromCPP = false;
        for (const auto& v : variables) {
            if (v.direction == "fromCPP") {
                hasFromCPP = true;
                out << "    ";
                if (v.type == "STRING") {
                    out << "char " << v.name << "[" << (v.size + 1) << "];  // STRING[" << v.size << "] + null\n";
                } else {
                    out << v.type << " " << v.name << ";\n";
                }
            }
        }
        if (!hasFromCPP) out << "    char dummy;  // Empty struct placeholder\n";
        out << "};\n\n";

        out << "struct ST_" << memoryName << " {\n";
        out << "    ST_ToCPP ToCPP;\n";
        out << "    ST_FromCPP FromCPP;\n";
        out << "    DINT LifePLC;\n";
        out << "    DINT LifeCPP;\n";
        out << "};\n\n";

        out << "#pragma pack(pop)\n";
    }

    // 3. Generate C++ Client
    void generateCppClient(const std::string& outputPath) {
        std::ofstream out(outputPath);
        
        std::string lowerName = memoryName;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        out << "#pragma once\n";
        out << "#include \"GenericSharedMemory.hpp\"\n";
        out << "#include \"" << lowerName << "_types.hpp\"\n\n";
        out << "// Auto-generated typed client for: " << memoryName << "\n\n";

        out << "class " << memoryName << "Client : public TypedSharedMemory<ST_" << memoryName << "> {\n";
        out << "public:\n";
        out << "    " << memoryName << "Client() : TypedSharedMemory<ST_" << memoryName << ">() {}\n\n";

        for (const auto& v : variables) {
            std::string capitalized = v.name;
            capitalized[0] = toupper(capitalized[0]);

            if (v.direction == "toCPP") {
                if (v.type == "STRING") {
                    out << "    const char* Get" << capitalized << "() const {\n";
                    out << "        return IsConnected() ? GetData()->ToCPP." << v.name << " : \"\";\n";
                    out << "    }\n\n";
                } else {
                    out << "    " << v.type << " Get" << capitalized << "() const {\n";
                    out << "        return IsConnected() ? GetData()->ToCPP." << v.name << " : 0;\n";
                    out << "    }\n\n";
                }
            }
        }

        for (const auto& v : variables) {
            std::string capitalized = v.name;
            capitalized[0] = toupper(capitalized[0]);

            if (v.direction == "fromCPP") {
                if (v.type == "STRING") {
                    out << "    void Set" << capitalized << "(const char* value) {\n";
                    out << "        if (IsConnected()) {\n";
                    out << "            strncpy(GetData()->FromCPP." << v.name << ", value, " << v.size << ");\n";
                    out << "            GetData()->FromCPP." << v.name << "[" << v.size << "] = 0;\n";
                    out << "        }\n";
                    out << "    }\n\n";
                } else {
                    out << "    void Set" << capitalized << "(" << v.type << " value) {\n";
                    out << "        if (IsConnected()) GetData()->FromCPP." << v.name << " = value;\n";
                    out << "    }\n\n";
                }
            }
        }

        out << "};\n";
    }

    // 4. Generate Webots Controller
    void generateWebotsController(const std::string& outputPath) {
        std::ofstream out(outputPath);
        
        std::string lowerName = memoryName;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        out << "// Auto-generated Webots controller for: " << memoryName << "\n\n";
        out << "#include <webots/Robot.hpp>\n";
        out << "#include <webots/Motor.hpp>\n";
        out << "#include <webots/PositionSensor.hpp>\n";
        out << "#include <iostream>\n";
        out << "#include \"" << lowerName << "_client.hpp\"\n\n";
        out << "using namespace webots;\n";
        out << "using namespace std;\n\n";
        out << "#define TIME_STEP 64\n\n";
        out << "int main(int argc, char **argv) {\n";
        out << "    Robot *robot = new Robot();\n";
        out << "    " << memoryName << "Client client;\n\n";
        out << "    cout << \"=== " << memoryName << " Webots Controller ===\" << endl;\n\n";

        out << "    // Get motors and sensors\n";
        for (const auto& v : variables) {
            if (v.direction == "toCPP" && v.type != "STRING") {
                out << "    Motor *" << v.name << "_motor = robot->getMotor(\"" << v.name << "\");\n";
            }
            // Assume fromCPP LREAL variables with "sensor" in name are PositionSensors
            if (v.direction == "fromCPP" && v.type == "LREAL") {
                std::string sensorName = v.name;
                // Simple heuristic: map variable name to device name
                out << "    PositionSensor *" << v.name << " = robot->getPositionSensor(\"" << sensorName << "\");\n";
                out << "    if (" << v.name << ") " << v.name << "->enable(TIME_STEP);\n";
            }
        }
        out << "\n";

        out << "    // Connect to CODESYS\n";
        out << "    while (!client.Connect(L\"OKZiJMksN52vWF53\")) {\n";
        out << "        cout << \"Waiting for CODESYS...\" << endl;\n";
        out << "        robot->step(TIME_STEP);\n";
        out << "    }\n";
        out << "    cout << \">> Connected!\" << endl;\n\n";

        out << "    // Main loop\n";
        out << "    while (robot->step(TIME_STEP) != -1) {\n";
        
        out << "        // Write commands (PLC -> Webots)\n";
        for (const auto& v : variables) {
            if (v.direction == "toCPP" && v.type != "STRING") {
                std::string capitalized = v.name;
                capitalized[0] = toupper(capitalized[0]);
                out << "        if (" << v.name << "_motor) " << v.name << "_motor->setPosition(client.Get" << capitalized << "());\n";
            }
        }

        out << "\n        // Read sensors (Webots -> PLC)\n";
        for (const auto& v : variables) {
             if (v.direction == "fromCPP" && v.type == "LREAL") {
                std::string capitalized = v.name;
                capitalized[0] = toupper(capitalized[0]);
                out << "        if (" << v.name << ") client.Set" << capitalized << "(" << v.name << "->getValue());\n";
            }
        }

        out << "        client.Sync();\n";
        out << "    }\n\n";
        out << "    delete robot;\n";
        out << "    return 0;\n";
        out << "}\n";
    }

    // 5. Generate Usage README
    void generateReadme(const std::string& outputPath) {
        std::ofstream out(outputPath);
        
        std::string lowerName = memoryName;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        out << "# " << memoryName << " - Generated Integration\n\n";
        out << "Auto-generated from JSON schema by Program12 Unified Generator\n\n";
        
        out << "## Generated Files\n\n";
        out << "- **CODESYS**: `codesys/" << lowerName << "_Types.xml`\n";
        out << "- **C++ Types**: `cpp/" << lowerName << "_types.hpp`\n";
        out << "- **C++ Client**: `cpp/" << lowerName << "_client.hpp`\n";
        out << "- **Webots Controller**: `webots/" << lowerName << "_controller.cpp`\n\n";
        
        out << "## CODESYS Setup\n\n";
        out << "1. **Import PLCopenXML:**\n";
        out << "   - Right-click project tree → 'Import PLCopenXML...'\n";
        out << "   - Select `codesys/" << lowerName << "_Types.xml`\n";
        out << "   - Structs appear in DUT (Data Unit Types)\n\n";
        out << "2. **Use in Program:**\n";
        out << "   ```structured-text\n";
        out << "   VAR\n";
        out << "       data : ST_" << memoryName << ";\n";
        out << "       fbShm : FB_GenericSharedMem;\n";
        out << "   END_VAR\n";
        out << "   \n";
        out << "   fbShm(\n";
        out << "       pData := ADR(data),\n";
        out << "       DataSize := SIZEOF(ST_" << memoryName << "),\n";
        out << "       sMemoryName := 'OKZiJMksN52vWF53',\n";
        out << "       Enable := TRUE\n";
        out << "   );\n";
        out << "   ```\n\n";
        
        out << "## C++ Client Setup\n\n";
        out << "1. **Include files:**\n";
        out << "   ```cpp\n";
        out << "   #include \"" << lowerName << "_client.hpp\"\n";
        out << "   ```\n\n";
        out << "2. **Use client:**\n";
        out << "   ```cpp\n";
        out << "   " << memoryName << "Client client;\n";
        out << "   client.Connect(L\"OKZiJMksN52vWF53\");\n";
        out << "   \n";
        out << "   auto* data = client.GetData();\n";
        out << "   client.Sync();\n";
        out << "   ```\n\n";
        
        out << "## Webots Setup\n\n";
        out << "1. **Create controller directory:**\n";
        out << "   `<webots_project>/controllers/" << lowerName << "/`\n\n";
        out << "2. **Copy files:**\n";
        out << "   - `webots/" << lowerName << "_controller.cpp`\n";
        out << "   - `cpp/" << lowerName << "_types.hpp`\n";
        out << "   - `cpp/" << lowerName << "_client.hpp`\n";
        out << "   - `GenericSharedMemory.hpp`\n";
        out << "   - `CodesysTypes.hpp`\n\n";
        out << "3. **Compile** and set as robot controller\n\n";
        
        out << "## Schema Variables\n\n";
        out << "| Name | Type | Direction | Purpose |\n";
        out << "|------|------|-----------|----------|\n";
        for (const auto& v : variables) {
            std::string typeStr = v.type;
            if (v.type == "STRING") typeStr += "[" + std::to_string(v.size) + "]";
            out << "| " << v.name << " | " << typeStr << " | " << v.direction << " | ";
            out << (v.direction == "toCPP" ? "PLC → C++" : "C++ → PLC") << " |\n";
        }
        
        out << "\n## UUIDs (for reference)\n\n";
        out << "- ST_" << memoryName << ": `" << uuidMain << "`\n";
        out << "- ST_ToCPP: `" << uuidToCPP << "`\n";
        out << "- ST_FromCPP: `" << uuidFromCPP << "`\n";
    }

    // Master generation method
    void generateAll(const std::string& baseName) {
        std::string lowerName = baseName;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        // Create output directories
        createDirectories("generated");
        createDirectories("generated\\codesys");
        createDirectories("generated\\webots");

        // Generate files - cpp and webots together since they're used together
        generatePLCopenXML("generated/codesys/" + lowerName + "_Types.xml");
        generateCppTypes("generated/webots/" + lowerName + "_types.hpp");
        generateCppClient("generated/webots/" + lowerName + "_client.hpp");
        generateWebotsController("generated/webots/" + lowerName + "_controller.cpp");
        generateReadme("generated/README_" + lowerName + ".md");

        // Copy generic libraries to webots folder for completeness
        copyFile("GenericSharedMemory.hpp", "generated/webots/GenericSharedMemory.hpp");
        copyFile("CodesysTypes.hpp", "generated/webots/CodesysTypes.hpp");
    }

};
