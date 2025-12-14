#include <ctype.h>
#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>

typedef StringView Cell;

typedef struct Row {
    Cell* data;
    size_t count;
    size_t capacity;
} Row;

typedef struct Rows {
    Row* data;
    size_t count;
    size_t capacity;
} Rows;


Rows transpose(Rows input_rows) {
    auto output_rows = (Rows){};
    return output_rows;
}

int isNewLine(int c) {
    return c == '\n';
}

int isSpace(int c) {
    return c == ' ';
}

int isContent(int c) {
    return !isSpace(c) && ! isNewLine(c);
}

#define STRING_VIEW_FROM_RANGE(range) ((StringView){(range).data, (range).count})

#define PARSE_WHILE(input_range, predicate, output_range) do { \
    (output_range).data = (input_range).data; \
    (output_range).count = 0; \
    while (!IS_EMPTY(input_range) && (predicate)(FIRST_ITEM(input_range))) { \
        output_range.count++; \
        DROP_FRONT(input_range); \
    } \
} while (0)

Cell parseCell(StringView* input_stream) {
    auto cell = (Cell){};
    DROP_FRONT_WHILE(*input_stream, isSpace);
    PARSE_WHILE(*input_stream, isContent, cell);
    DROP_FRONT_WHILE(*input_stream, isSpace);
    return cell;
}

Row parseRow(StringView* input_stream) {
    auto row = (Row){};
    while (true) {
        auto cell = parseCell(input_stream);
        if (IS_EMPTY(cell)) {
            break;
        }
        APPEND(row, cell);
    }
    DROP_FRONT_WHILE(*input_stream, isNewLine);
    return row;
}

Rows parseRows(StringView* input_stream) {
    auto rows = (Rows){};
    while (true) {
        auto row = parseRow(input_stream);
        if (IS_EMPTY(row)) {
            break;
        }
        APPEND(rows, row);
    }
    return rows;
}

int parseInt(StringView text) {
    auto result = 0;
    while (!IS_EMPTY(text) && isdigit(FIRST_ITEM(text))) {
        result = result * 10 + (FIRST_ITEM(text) - '0');
        DROP_FRONT(text);
    }
    return result;
}

long parseLong(StringView text) {
    long result = 0;
    while (!IS_EMPTY(text) && isdigit(FIRST_ITEM(text))) {
        result = result * 10 + (FIRST_ITEM(text) - '0');
        DROP_FRONT(text);
    }
    return result;
}

char parseChar(StringView text) {
    return IS_EMPTY(text) ? '\0' : FIRST_ITEM(text);
}

int main() {
    auto file_path = "day06.txt";
    auto text = read_text_file(file_path);
    auto text_view = STRING_VIEW_FROM_RANGE(text);
    auto rows = parseRows(&text_view);
    printf("Test\n");
    long result = 0;
    FOR_INDEX(i, FIRST_ITEM(rows)) {
        auto a = parseLong(rows.data[0].data[i]);
        auto b = parseLong(rows.data[1].data[i]);
        auto c = parseLong(rows.data[2].data[i]);
        auto d = parseLong(rows.data[3].data[i]);
        auto operation = parseChar(rows.data[4].data[i]);
        if (operation == '*') {
            result += a * b * c * d;
        }
        if (operation == '+') {
            result += a + b + c + d;
        }
    }
    printf("Result: %ld\n", result);
}
