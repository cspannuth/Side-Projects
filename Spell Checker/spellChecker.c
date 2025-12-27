// C library imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
static int choice = 0;  // Global choice used by both suggestWord and WindowProc

typedef struct tree_node {
    int end;
    struct tree_node* next[26];
} trie;

char* suggestWord(char* misWord, HWND hwnd);
int findDistance(char* word1, char* word2, int len1, int len2);
void addToDictionary(char* word, HWND hwnd);
void newLineDictionary(HWND hwnd);

// Trie insertion
void addNode(trie *root, const char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        const int index = tolower(word[i]) - 'a';
        if (index < 0 || index >= 26) continue;

        if (root->next[index] == NULL) {
            trie* newNode = calloc(1, sizeof(trie));
            newNode->end = 0;
            root->next[index] = newNode;
        }
        root = root->next[index];
    }
    root->end = 1;
}

// Trie search
int search(trie* root, char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        int index = tolower(word[i]) - 'a';
        if (index < 0 || index >= 26 || root->next[index] == NULL)
            return -1;
        root = root->next[index];
    }
    return root->end ? 1 : -1;
}

// Levenshtein distance
int findDistance(char* word1, char* word2, int len1, int len2) {
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;

    int* prev = malloc((len2 + 1) * sizeof(int));
    int* curr = malloc((len2 + 1) * sizeof(int));

    for (int i = 0; i <= len2; i++) prev[i] = i;

    for (int i = 1; i <= len1; i++) {
        curr[0] = i;
        for (int j = 1; j <= len2; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + min(curr[j - 1], min(prev[j], prev[j - 1]));
            }
        }
        int* temp = prev;
        prev = curr;
        curr = temp;
    }

    int result = prev[len2];
    free(prev);
    free(curr);
    return result;
}

void addToDictionary(char* word, HWND hwnd) {
    FILE* dictionary = fopen("dictionary.txt", "a");
    if (dictionary == NULL) {
        MessageBox(hwnd, "Error opening dictionary", "Error", MB_OK);
        return;
    }
    fprintf(dictionary, "%s\n", word);
    fclose(dictionary);
}

void newLineDictionary(HWND hwnd) {
    FILE* dictionary = fopen("dictionary.txt", "a");
    if (dictionary == NULL) {
        MessageBox(hwnd, "Error opening dictionary", "Error", MB_OK);
        return;
    }
    fprintf(dictionary, "\n");
    fclose(dictionary);
}

char* suggestWord(char* misWord, HWND hwnd) {
    int dists[3] = { 100, 100, 100 };
    static char word1[20] = "", word2[20] = "", word3[20] = "";
    FILE* fptr = fopen("dictionary.txt", "r");
    if (fptr == NULL) {
        MessageBox(hwnd, "Error opening dictionary file", "Error", MB_OK);
        return misWord;
    }

    char word[100];
    while (fgets(word, sizeof(word), fptr)) {
        word[strcspn(word, "\n")] = '\0';
        int len1 = strlen(misWord), len2 = strlen(word);
        if (abs(len1 - len2) < 2) {
            int dist = findDistance(misWord, word, len1, len2);
            if (dist < dists[0]) {
                dists[2] = dists[1]; strcpy(word3, word2);
                dists[1] = dists[0]; strcpy(word2, word1);
                dists[0] = dist;     strncpy(word1, word, sizeof(word1) - 1); word1[sizeof(word1) - 1] = '\0';
            } else if (dist < dists[1]) {
                dists[2] = dists[1]; strcpy(word3, word2);
                dists[1] = dist;     strncpy(word2, word, sizeof(word2) - 1); word2[sizeof(word2) - 1] = '\0';
            } else if (dist < dists[2]) {
                dists[2] = dist;     strncpy(word3, word, sizeof(word3) - 1); word3[sizeof(word3) - 1] = '\0';
            }
        }
    }
    fclose(fptr);

    char misType[256] = "You mistyped the word: '";
    strcat(misType, misWord);
    strcat(misType, "' Did you mean?");

    HINSTANCE hInstance = GetModuleHandle(NULL);
    choice = 0;

    CreateWindow("STATIC", misType, WS_EX_TRANSPARENT | WS_TABSTOP | WS_VISIBLE | WS_CHILD, 0, 75, 300, 50, hwnd, (HMENU)90, hInstance, NULL);
    CreateWindow("BUTTON", word1, WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 5, 125, 75, 30, hwnd, (HMENU)91, hInstance, NULL);
    CreateWindow("BUTTON", word2, WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 5, 185, 75, 30, hwnd, (HMENU)92, hInstance, NULL);
    CreateWindow("BUTTON", word3, WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 5, 245, 75, 30, hwnd, (HMENU)93, hInstance, NULL);
    CreateWindow("BUTTON", "Add word to dictionary", WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 200, 275, 150, 30, hwnd, (HMENU)94, hInstance, NULL);
    CreateWindow("BUTTON", "New set of words", WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 200, 225, 150, 30, hwnd, (HMENU)95, hInstance, NULL);
    HWND hBackground = CreateWindow("STATIC", NULL, WS_EX_TRANSPARENT | WS_CHILD | WS_VISIBLE | SS_BITMAP, 0, 0, 200, 100, hwnd, (HMENU)6, hInstance, NULL);
    HBITMAP hBackgroundBmp = (HBITMAP)LoadImage(NULL, "errorBackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SendMessage(hBackground, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBackgroundBmp);

    MSG msg;
    while (choice == 0) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                choice = -1;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }

    DestroyWindow(GetDlgItem(hwnd, 90));
    DestroyWindow(GetDlgItem(hwnd, 91));
    DestroyWindow(GetDlgItem(hwnd, 92));
    DestroyWindow(GetDlgItem(hwnd, 93));
    DestroyWindow(GetDlgItem(hwnd, 94));
    DestroyWindow(GetDlgItem(hwnd, 95));

    switch (choice) {
        case 1: return word1;
        case 2: return word2;
        case 3: return word3;
        case 4: addToDictionary(misWord, hwnd); return misWord;
        case -1: PostQuitMessage(0);
        default: return word1;
    }
}

void markErrors(trie* root, FILE* fptr, HWND hwnd) {
    FILE *newFile = fopen("corrections.txt", "w");
    if (newFile == NULL) {
        MessageBox(hwnd, "Error creating corrections file", "Error", MB_OK);
        return;
    }

    char text[1024];
    while (fgets(text, sizeof(text), fptr)) {
        int j = 0;
        while (text[j] != '\0') {
            if (isalpha(text[j])) {
                char word[32];
                int i = 0;
                while (isalpha(text[j]) && i < sizeof(word) - 1) {
                    word[i++] = text[j++];
                }
                word[i] = '\0';

                if (search(root, word) == -1) {
                    char* suggWord = suggestWord(word, hwnd);
                    fprintf(newFile, "%s", suggWord);
                } else {
                    fprintf(newFile, "%s", word);
                }
            } else {
                char other[32];
                int i = 0;
                while (!isalpha(text[j]) && text[j] != '\0' && i < sizeof(other) - 1) {
                    other[i++] = text[j++];
                }
                other[i] = '\0';
                fprintf(newFile, "%s", other);
            }
        }
    }

    fclose(newFile);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HINSTANCE editInst;
    static HWND hEdit;
    static HWND hOkButton;
    static FILE* essay;

    switch (msg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1:
                    editInst = GetModuleHandle(NULL);
                    hEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                         50, 200, 200, 25, hwnd, (HMENU)4, editInst, NULL);
                    hOkButton = CreateWindow("BUTTON", "OK", WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
                                             260, 200, 50, 25, hwnd, (HMENU)3, editInst, NULL);
                    break;

                case 2: {
                    FILE *fptr = fopen("dictionary.txt", "r");
                    if (fptr == NULL) {
                        MessageBox(hwnd, "Error opening dictionary file", "Error", MB_OK);
                        return 1;
                    }

                    trie* root = calloc(1, sizeof(trie));
                    char word[100];
                    while (fgets(word, sizeof(word), fptr)) {
                        word[strcspn(word, "\n")] = '\0';
                        addNode(root, word);
                    }

                    ShowWindow(GetDlgItem(hwnd, 1), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, 2), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, 5), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, 6), SW_HIDE);

                    markErrors(root, essay, hwnd);
                    fclose(fptr);
                    free(root);

                    ShowWindow(GetDlgItem(hwnd, 1), SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, 2), SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, 5), SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, 6), SW_SHOW);

                    MessageBox(hwnd, "Spell Check Complete!", "Success!", MB_OK);
                    break;
                }

                case 3: {
                    static char buffer[256];
                    GetWindowText(GetDlgItem(hwnd, 4), buffer, sizeof(buffer));
                    essay = fopen(buffer, "r");
                    if (essay == NULL) {
                        MessageBox(hwnd, "Error opening file", "Error", MB_OK);
                    }
                    DestroyWindow(GetDlgItem(hwnd, 4));
                    DestroyWindow(GetDlgItem(hwnd, 3));
                    char txt[300] = "Path: ";
                    strcat(txt, buffer);
                    SetWindowText(GetDlgItem(hwnd, 5), txt);
                    break;
                }

                case 91: choice = 1; break;
                case 92: choice = 2; break;
                case 93: choice = 3; break;
                case 94: choice = 4; break;
                case 95: {

                    }

            }
            break;

        case WM_DESTROY:
            choice = -1;
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
            return 0;
        }

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SpellChecker";
    wc.hIcon = (HICON)LoadImage(NULL, "logoIco.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, "SpellChecker", "SpellChecker",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 380,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    // Buttons
    CreateWindow("BUTTON", "Essay Path", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        200, 150, 100, 30, hwnd, (HMENU)1, hInstance, NULL);
    CreateWindow("BUTTON", "Mark Errors", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        50, 150, 100, 30, hwnd, (HMENU)2, hInstance, NULL);
    CreateWindow("STATIC", "Path: None", WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        50, 250, 200, 30, hwnd, (HMENU)5, hInstance, NULL);
    HWND hLogo = CreateWindow("STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP,
        50, 0, 200, 100, hwnd, (HMENU)6, hInstance, NULL);
    HBITMAP hBmp = (HBITMAP)LoadImage(NULL, "logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SendMessage(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

//Need to implement word not listed button.
//Side quest, find a better dictionary
// Improve visuals on mark errors function
// Look over unreachable code from terminal version, and see what can be simplified
// Issue with app getting stuck not regenerating prompts after completing spell check