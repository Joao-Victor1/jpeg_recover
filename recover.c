#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const uint8_t jpeg_signature[] = {0xFF, 0xD8, 0xFF, 0xE0};

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Uso correto: ./recover <imagem_forense>\n");
        return 1;
    }

    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Não foi possível abrir a imagem forense para leitura.\n");
        return 1;
    }

    int jpeg_count = 0;

    FILE *output_file = NULL;
    char file_name[8];

    uint8_t buffer[512];

    while (fread(buffer, sizeof(buffer), 1, input_file) == 1) {

        if (memcmp(buffer, jpeg_signature, sizeof(jpeg_signature)) == 0) {
            if (output_file != NULL) {
                fclose(output_file);
            }

            sprintf(file_name, "%03d.jpg", jpeg_count);
            output_file = fopen(file_name, "wb");
            if (output_file == NULL) {
                fprintf(stderr, "Não foi possível criar o arquivo JPEG %s.\n", file_name);
                fclose(input_file);
                return 1;
            }

            jpeg_count++;
        }

        if (output_file != NULL) {
            fwrite(buffer, sizeof(buffer), 1, output_file);
        }
    }

    if (output_file != NULL) {
        fclose(output_file);
    }
    fclose(input_file);

    return 0;
}
