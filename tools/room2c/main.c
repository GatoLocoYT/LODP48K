#include <stdio.h>
#include <stdint.h>

#include "../common/bmp.h"

#define TILE_EMPTY 0
#define TILE_WALL  1

#define TILE_SIZE 8

#define ROOM_WIDTH 16
#define ROOM_HEIGHT 16
#define ROOM_COUNT 12

static uint8_t ColorToTile(uint32_t color)
{
    switch (color)
    {
        case 0xFFFF00FF:
            return TILE_WALL;

        default:
            return TILE_EMPTY;
    }
}

static void WriteRoom(
    FILE* out,
    const Bitmap* bmp,
    int roomNumber)
{
    fprintf(
        out,
        "static const uint8_t room_%03d[] =\n{\n",
        roomNumber
    );

    for (int tileY = 0; tileY < ROOM_HEIGHT; tileY++)
    {
        fprintf(
            out,
            "    // Row %d\n    ",
            tileY
        );

        for (int tileX = 0; tileX < ROOM_WIDTH; tileX++)
        {
            int pixelX = tileX * TILE_SIZE;
            int pixelY = tileY * TILE_SIZE;

            uint32_t color =
                bmp->pixels[pixelY * bmp->width + pixelX];

            uint8_t tile =
                ColorToTile(color);

            fprintf(
                out,
                "%u",
                tile
            );

            if (!(tileY == ROOM_HEIGHT - 1 &&
                  tileX == ROOM_WIDTH - 1))
            {
                fprintf(out, ", ");
            }
        }

        fprintf(out, "\n\n");
    }

    fprintf(out, "};\n\n");
}

int main(void)
{
    FILE* out = fopen("../generated/rooms.h", "w");

    if (out == NULL)
    {
        printf("Error: could not create rooms.h\n");
        return 1;
    }

    fprintf(
        out,
        "#ifndef ROOMS_H\n"
        "#define ROOMS_H\n\n"

        "#include <stdint.h>\n\n"

        "#define ROOM_WIDTH 16\n"
        "#define ROOM_HEIGHT 16\n"
        "#define ROOM_COUNT 12\n\n"

        "#define TILE_EMPTY 0\n"
        "#define TILE_WALL  1\n\n"
    );

    char path[256];

    for (int room = 1; room <= ROOM_COUNT; room++)
    {
        sprintf(
            path,
            "../../assets/rooms/room_%03d.bmp",
            room
        );

        printf("Loading %s\n", path);

        Bitmap bmp = {0};

        if (!BMP_Load(path, &bmp))
        {
            printf(
                "Failed to load room %03d.\n",
                room
            );

            fclose(out);
            return 1;
        }

        printf(
            "  %dx%d\n",
            bmp.width,
            bmp.height
        );

        WriteRoom(
            out,
            &bmp,
            room
        );

        BMP_Free(&bmp);
    }

    fprintf(
        out,
        "static const uint8_t* gRooms[ROOM_COUNT] =\n"
        "{\n"
    );

    for (int room = 1; room <= ROOM_COUNT; room++)
    {
        fprintf(
            out,
            "    room_%03d",
            room
        );

        if (room != ROOM_COUNT)
        {
            fprintf(out, ",");
        }

        fprintf(out, "\n");
    }

    fprintf(
        out,
        "};\n\n"

        "#endif\n"
    );

    fclose(out);

    printf("\nrooms.h generated successfully.\n");

    return 0;
}