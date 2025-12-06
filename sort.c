#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void smth_wrong(){
    puts("что-то пошло не так");
}

void printf_arr(const int* arr, const size_t len){
    printf("%d", arr[0]);
    for (size_t i = 1; i < len; i++){
        printf(", %d", arr[i]);
    }
}

void swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void insertion_sort(int* arr, const size_t len){
    for(size_t i = 1; i < len; i++){
        int save = arr[i];
        size_t j = i - 1;
        if (arr[j] <= save) continue;
        for(; j >= 0 && arr[j] > save; j--){
            arr[j+1] = arr[j];
        }
        arr[j+1] = save;
    }
}

void shell_sort(int arr[], const size_t len){
    const int* after_end = arr + len;
    for(size_t d = len/2; d >= 1; d /= 2){
        for (int* cur = arr + d; cur < after_end; cur++){
            int save = *cur;
            int* p = cur - d;
            if (*p <= save) continue;
            for(; p >= arr && *p > save; p -= d){
                *(p + d) = *p;
            }
            *(p+d) = save;
        }
    }
}

bool check_sort(const int* arr, const size_t len){
    for (size_t i = 1; i < len; i++){
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

int iteractive_sort(){
    char sort_code;
    
    fputs(   "Выберите метод сортировки (введите цифру или букву метода):\n"
            "1. Вставками (i)\n"
            "2. Шелла (s)\n"
            "> "
    , stdout);
    scanf(" %c", &sort_code);
    switch (sort_code){
        case '1': sort_code = 'i'; break;
        case '2': sort_code = 's'; break;
        case 'i': case 's': break;
        default: smth_wrong(); return 1;
    }


    char input_method;
    fputs(   "Выберите метод ввода (введите цифру или букву метода):\n"
            "1. Ввод с клавиатуры (i)\n"
            "2. Генерация случайных чисел (r)\n"
            "3. Чтение из файла (f)\n"
            "> "
    , stdout);
    scanf(" %c", &input_method);
    switch (input_method){
        case '1': input_method = 'i'; break;
        case '2': input_method = 'r'; break;
        case '3': input_method = 'f'; break;
        case 'i': case 'r': case 'f': break;
        default: smth_wrong(); return 1;
    }

    size_t arr_len;
    int* arr;

    fputs("Введите размер > ", stdout);
    scanf("%zi", &arr_len);
    arr = malloc(sizeof(int) * arr_len);
    if (arr == NULL){ smth_wrong(); exit(1); }

    if (input_method == 'r'){
        long tmp, from, upto;
        fputs("Введите число seed для случайности (отрицательное число - будет использовано время)\n> ",
            stdout);
        scanf("%li", &tmp);
        if (tmp < 0)
            srand(time(NULL));
        else
            srand((unsigned) tmp);

        fputs("Числа от (вкл)\n> ",stdout);
        scanf("%li", &from);
        
        fputs("Числа до (не вкл)\n> ",stdout);
        scanf("%li", &upto);


        for(size_t i = 0; i < arr_len; i++){
            arr[i] = (rand() + from) % upto;
        }
    }else{
        FILE* f;
        if (input_method == 'f'){
            fputs("Введите имя файла > ", stdout);
            static char filename[256];
            scanf("%256s", filename);
            f = fopen(filename, "r");
        }else if (input_method == 'i'){
            f = stdin;
        }else{
            smth_wrong(); return 1;
        }

        if (f == NULL){
            smth_wrong(); return 1;
        }

        if (input_method == 'i')
            fputs("Введите числа через пробельные символы > ", stdout);

        for(size_t i = 0; i < arr_len; i++){
            fscanf(f, "%i", &arr[i]);
        }
    }

    fputs("Введено: ", stdout); printf_arr(arr, arr_len); puts("");

    switch(sort_code){
        case 'i': 
            insertion_sort(arr, arr_len);
            break;
        case 's': 
            shell_sort(arr, arr_len);
            break;
    }

    char* flag_s = "false";
                //true
    if (check_sort(arr, arr_len)) flag_s = "true";

    printf("Отсортировано (%s): ", flag_s); printf_arr(arr, arr_len); puts("");

    
    puts("Enter чтобы продолжить");
    getchar();getchar();

}
