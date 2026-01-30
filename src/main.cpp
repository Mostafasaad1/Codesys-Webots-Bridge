#include <iostream>
#include "UnifiedCodeGenerator.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: unified_codegen <schema.json>" << std::endl;
        std::cerr << "Example: unified_codegen schemas/puma560.json" << std::endl;
        return 1;
    }

    try {
        UnifiedCodeGenerator gen;
        
        std::cout << "============================================\n";
        std::cout << " Unified Robot Code Generator\n";
        std::cout << " Complete CODESYS + Webots Automation\n";
        std::cout << "============================================\n\n";
        
        std::cout << "Loading schema: " << argv[1] << std::endl;
        gen.loadSchema(argv[1]);
        
        std::cout << "Schema: " << gen.memoryName << std::endl;
        std::cout << "Variables: " << gen.variables.size() << std::endl;
        std::cout << "UUIDs generated: " << gen.uuidMain.substr(0, 8) << "...\n\n";

        std::string lowerName = gen.memoryName;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        std::cout << "Generating all files...\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        gen.generateAll(lowerName);
        
        std::cout << "✓ PLCopenXML:        codesys/" << lowerName << "_Types.xml\n";
        std::cout << "✓ C++ Types:         webots/" << lowerName << "_types.hpp\n";
        std::cout << "✓ C++ Client:        webots/" << lowerName << "_client.hpp\n";
        std::cout << "✓ Webots Controller: webots/" << lowerName << "_controller.cpp\n";
        std::cout << "✓ Usage README:      README_" << lowerName << ".md\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
        
        std::cout << "✨ Generation complete!\n\n";
        
        std::cout << "Next steps:\n";
        std::cout << "  1. CODESYS: Import PLCopenXML from generated/codesys/\n";
        std::cout << "  2. Webots:  Copy all files from generated/webots/ to controller\n";
        std::cout << "  3. C++:     Use client from generated/webots/ (standalone)\n\n";
        
        std::cout << "See generated/README_" << lowerName << ".md for detailed instructions.\n";

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
