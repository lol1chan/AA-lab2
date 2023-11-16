#include "Header.h"

Base64::Base64() {
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}


void Encoder :: EncodeTriplet(const std::string& triplet) {
    std::string result = "====";

    int k = triplet.at(0) >> 2;
    result.at(0) = alphabet.at(k);

    k = (triplet.at(0) & 3) << 4;
    k = k | (triplet.at(1) >> 4);
    result.at(1) = alphabet.at(k);

    k = (triplet.at(1) & 0xF) << 2;
    k = k | (triplet.at(2) >> 6);
    result.at(2) = alphabet.at(k);

    k = triplet.at(2) & 0x3F;
    result.at(3) = alphabet.at(k);
    
    coded += result; 

}

void Encoder::EncodeDuplet(const std::string& duplet) {
    std::string result = "====";

    int k = duplet.at(0) >> 2;
    result.at(0) = alphabet.at(k);

    k = (duplet.at(0) & 3) << 4;
    k = k | (duplet.at(1) >> 4);
    result.at(1) = alphabet.at(k);

    k = (duplet.at(1) & 0xF) << 2;
    result.at(2) = alphabet.at(k);

    coded += result;

}

void Encoder::EncodeSymbol(const char& symbol) {
    std::string result = "====";

    int k = symbol >> 2;
    result.at(0) = alphabet.at(k);

    k = (symbol & 3) << 4;
    result.at(1) = alphabet.at(k);

    coded += result;

}

int Decoder::getPos(const char& symbol) {
    for (int i = 0; i < 64; i++) {
        if (alphabet.at(i) == symbol)
            return i;
    
    }

    return -1;
}

int Decoder::DecodeTriplet(const std::string& triplet) {
    std::string result = "---";

    int k = getPos(triplet.at(0));
    if (k == -1) {
        return 1;
    }

    result.at(0)= k << 2;
    k = getPos(triplet.at(1));
    if (k == -1) {
        return 2;
    }

    result.at(0) = result.at(0) | (k >> 4);
    result.at(1) = (k << 4) & 0xFF;
    k = getPos(triplet.at(2));
    if (k == -1) {
        return 3;
    }

    result.at(1) = result.at(1) | (k >> 2);
    result.at(2) = (k << 6) & 0xFF;
    k = getPos(triplet.at(3));
    if (k == -1) {
        return 4;
    }

    result.at(2) = result.at(2) | k;
    output += result;
    return 0;
}

int Decoder::DecodeDuplet(const std::string& duplet) {
    std::string result = "--";

    int k = getPos(duplet.at(0));
    if (k == -1) {
        return 1;
    }

    result.at(0) = k << 2;
    k = getPos(duplet.at(1));
    if (k == -1) {
        return 2;
    }

    result.at(0) = result.at(0) | (k >> 4);
    result.at(1) = (k << 4) & 0xFF;
    k = getPos(duplet.at(2));
    if (k == -1) {
        return 3;
    }
    result.at(1) = result.at(1) | k >> 2;
    

    output += result; 
    return 0;

}

int Decoder::DecodeSymbol(const std::string& symbol) {

    std::string result = "-";

    int k = getPos(symbol.at(0));
    if (k == -1) {
        return 1;
    }

    result.at(0) = k << 2;
    k = getPos(symbol.at(1));
    if (k == -1) {
        return 2;
    }

    result.at(0) = result.at(0) | (k >> 4);
    
    output += result;
    return 0;
}


void Encoder::EncodeFile(const std::string& FilePath) {
    inputFile.open(FilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << FilePath << std::endl;
        return;
    }

    char buffer[3];
    while (inputFile.read(buffer, 3)) {
        std::string triplet(buffer, inputFile.gcount());
        EncodeTriplet(triplet);
    }

    
    std::string lastTriplet(buffer, inputFile.gcount());
    if (!lastTriplet.empty()) {
        if (lastTriplet.size() == 2) {          
            EncodeDuplet(lastTriplet);
        }
        else if (lastTriplet.size() == 1) {
            EncodeSymbol(lastTriplet[0]);
        }
    }

    inputFile.close();
}


int Decoder::DecodeFile(const std::string& FilePath) {
    inputFile.open(FilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << FilePath << std::endl;
        return 1;
    }

    char buffer[4];
    while (inputFile.read(buffer, 4)) {
        std::string quadruplet(buffer, inputFile.gcount());

        std::string remaining(buffer, inputFile.gcount());


            if ((remaining[2] == '=') && (remaining[3] == '=')) {
                remaining.erase(3);
                remaining.erase(2);
                DecodeSymbol(remaining);
                continue;
            }
            else if (remaining[3] == '=') {
                remaining.erase(3);
                DecodeDuplet(remaining);
                continue;

            }

        
        int result = DecodeTriplet(quadruplet);
        if (result != 0) {
            std::cerr << "Error decoding quadruplet in file: " << FilePath << std::endl;
            inputFile.close();
            return result;
        }
    }


    inputFile.close();
    return 0;
}

void Encoder::DataToFile(const std::string& FilePath) {
    outputFile.open(FilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << FilePath << std::endl;
        return;
    }

    outputFile << coded;

    outputFile.close();
}

void Decoder::DataToFile(const std::string& FilePath) {
    outputFile.open(FilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << FilePath << std::endl;
        return;
    }

    outputFile << output;

    outputFile.close();
}

void Encoder::ClearCoded() {
    coded.clear();
}

void Decoder::ClearOutput() {
    output.clear();
}

int main() {
    Encoder encoder;
    Decoder decoder;

    encoder.EncodeFile("input.txt");
    encoder.DataToFile("encoded.txt");
    encoder.ClearCoded();

    

    int decodeResult = decoder.DecodeFile("encoded.txt");
    if (decodeResult == 0) {
        decoder.DataToFile("decoded.txt");
        decoder.ClearOutput();
    }
    else {
        std::cerr << "Decoding failed with error code: " << decodeResult << std::endl;
    }

    Encoder encoderr;
    Decoder decoderr;

    encoderr.EncodeFile("input1.txt");
    encoderr.DataToFile("encoded1.txt");
    encoderr.ClearCoded();

    int decodeResultr = decoderr.DecodeFile("encoded1.txt");
    if (decodeResultr == 0) {
        decoderr.DataToFile("decoded1.txt");
        decoderr.ClearOutput();
    }
    else {
        std::cerr << "Decoding failed with error code: " << decodeResultr << std::endl;
    }

    Encoder encoders;
    Decoder decoders;

    encoders.EncodeFile("input2.txt");
    encoders.DataToFile("encoded2.txt");
    encoders.ClearCoded();

    int decodeResults = decoders.DecodeFile("encoded2.txt");
    if (decodeResults == 0) {
        decoders.DataToFile("decoded2.txt");
        decoders.ClearOutput();
    }
    else {
        std::cerr << "Decoding failed with error code: " << decodeResults << std::endl;
    }

    return 0;
}
