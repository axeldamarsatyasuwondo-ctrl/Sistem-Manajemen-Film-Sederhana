#include <iostream>
using namespace std;

// ================= INPUT AMAN =================
void clearInput() {
    cin.clear();
    cin.ignore(1000, '\n');
}

string inputString(string pesan) {
    string data;
    while (true) {
        cout << pesan;
        getline(cin, data);
        if (!data.empty()) return data;
        cout << "Input tidak boleh kosong!\n";
    }
}

int inputInt(string pesan, int minVal, int maxVal) {
    int x;
    while (true) {
        cout << pesan;
        cin >> x;

        if (cin.fail()) {
            clearInput();
            cout << "Harus angka!\n";
            continue;
        }

        if (x < minVal || x > maxVal) {
            cout << "Range " << minVal << "-" << maxVal << endl;
            continue;
        }

        clearInput();
        return x;
    }
}

float inputFloat(string pesan, float minVal, float maxVal) {
    float x;
    while (true) {
        cout << pesan;
        cin >> x;

        if (cin.fail()) {
            clearInput();
            cout << "Harus angka!\n";
            continue;
        }

        if (x < minVal || x > maxVal) {
            cout << "Range " << minVal << "-" << maxVal << endl;
            continue;
        }

        clearInput();
        return x;
    }
}

//  STRUCT 
struct Movie {
    string nama;
    string genre[5];
    int genreCount;
    float rating;

    Movie* next;
    Movie* left;
    Movie* right;

    Movie* related[5];
    int relatedCount;
};

struct Genre {
    string nama;
    Movie* films[100];
    int filmCount;
};

//  GLOBAL
Movie* head = NULL;
Movie* root = NULL;

Movie* stackArr[100];
int top = -1;

Movie* queueArr[100];
int front = 0, rear = -1;

Movie* hashTable[100];

Genre genreList[10];
int genreCountGlobal = 0;

// INIT
void initHash() {
    for (int i = 0; i < 100; i++)
        hashTable[i] = NULL;
}

void initGenre() {
    string g[] = {"Action","Drama","Horror","Comedy","Fantasy","Romance","SciFi","Mystery"};
    genreCountGlobal = 8;

    for (int i = 0; i < 8; i++) {
        genreList[i].nama = g[i];
        genreList[i].filmCount = 0;
    }
}

// HASH
int hashFunc(string s) {
    int sum = 0;
    for (int i = 0; i < s.length(); i++)
        sum += s[i];
    return sum % 100;
}

Movie* searchHash(string nama) {
    int idx = hashFunc(nama), start = idx;

    while (hashTable[idx] != NULL) {
        if (hashTable[idx]->nama == nama)
            return hashTable[idx];

        idx = (idx + 1) % 100;
        if (idx == start) break;
    }
    return NULL;
}

//  CREATE
Movie* createMovie(string nama, float rating) {
    Movie* m = new Movie;
    m->nama = nama;
    m->rating = rating;
    m->genreCount = 0;
    m->next = NULL;
    m->left = m->right = NULL;
    m->relatedCount = 0;
    return m;
}

// ================= TAMBAH =================
void tambahFilm() {
    string nama = inputString("Nama: ");
    float rating = inputFloat("Rating: ", 0, 10);

    Movie* m = createMovie(nama, rating);

    cout << "Pilih Genre:\n";
    for (int i = 0; i < genreCountGlobal; i++)
        cout << i+1 << ". " << genreList[i].nama << endl;

    int jumlah = inputInt("Jumlah genre: ", 1, 5);

    for (int i = 0; i < jumlah; i++) {
        int pilih = inputInt("Pilih: ", 1, genreCountGlobal) - 1;

        bool sudah = false;
        for (int j = 0; j < m->genreCount; j++)
            if (m->genre[j] == genreList[pilih].nama)
                sudah = true;

        if (!sudah) {
            m->genre[m->genreCount++] = genreList[pilih].nama;
            genreList[pilih].films[genreList[pilih].filmCount++] = m;
        } else {
            cout << "Genre sudah dipilih!\n";
            i--;
        }
    }

    // linked list
    if (!head) head = m;
    else {
        Movie* t = head;
        while (t->next) t = t->next;
        t->next = m;
    }

    // stack
    stackArr[++top] = m;

    // hash
    int idx = hashFunc(nama);
    while (hashTable[idx] != NULL)
        idx = (idx + 1) % 100;
    hashTable[idx] = m;

    // bst
    if (!root) root = m;
    else {
        Movie* cur = root;
        while (true) {
            if (rating < cur->rating) {
                if (!cur->left) { cur->left = m; break; }
                cur = cur->left;
            } else {
                if (!cur->right) { cur->right = m; break; }
                cur = cur->right;
            }
        }
    }

    cout << "Berhasil!\n";
}

// ================= TAMPIL =================
void tampilFilm() {
    Movie* t = head;
    while (t) {
        cout << t->nama << " | " << t->rating << " | ";
        for (int i = 0; i < t->genreCount; i++)
            cout << t->genre[i] << " ";
        cout << endl;
        t = t->next;
    }
}

// ================= SORT =================
void swapFull(Movie* a, Movie* b) {
    swap(a->nama, b->nama);
    swap(a->rating, b->rating);

    swap(a->genreCount, b->genreCount);
    for (int i = 0; i < 5; i++)
        swap(a->genre[i], b->genre[i]);
}

void sortFilm() {
    for (Movie* i = head; i; i = i->next) {
        for (Movie* j = i->next; j; j = j->next) {
            if (i->rating < j->rating)
                swapFull(i, j);
        }
    }
    cout << "Sorted!\n";
}

//  SEARCH
void cariFilm() {
    string nama = inputString("Cari: ");
    Movie* m = searchHash(nama);

    if (m) cout << "Ketemu: " << m->nama << endl;
    else cout << "Tidak ada\n";
}

//  DELETE
void hapusLinked(Movie* target) {
    if (head == target) {
        head = head->next;
        return;
    }

    Movie* t = head;
    while (t->next && t->next != target)
        t = t->next;

    if (t->next) t->next = target->next;
}

void hapusHash(Movie* target) {
    int idx = hashFunc(target->nama);
    while (hashTable[idx]) {
        if (hashTable[idx] == target) {
            hashTable[idx] = NULL;
            return;
        }
        idx = (idx + 1) % 100;
    }
}

void hapusGenre(Movie* target) {
    for (int i = 0; i < genreCountGlobal; i++) {
        for (int j = 0; j < genreList[i].filmCount; j++) {
            if (genreList[i].films[j] == target) {
                for (int k = j; k < genreList[i].filmCount-1; k++)
                    genreList[i].films[k] = genreList[i].films[k+1];
                genreList[i].filmCount--;
                j--;
            }
        }
    }
}

void undoFilm() {
    if (top == -1) {
        cout << "Kosong\n";
        return;
    }

    Movie* m = stackArr[top--];

    hapusLinked(m);
    hapusHash(m);
    hapusGenre(m);

    cout << "Dihapus: " << m->nama << endl;
    delete m;
}

// ================= QUEUE =================
void tambahAntrian() {
    string nama = inputString("Film: ");
    Movie* m = searchHash(nama);

    if (!m) return;

    queueArr[++rear] = m;
}

void prosesAntrian() {
    if (front > rear) return;

    cout << "Nonton: " << queueArr[front++]->nama << endl;
}

// ================= BST =================
void inorder(Movie* n) {
    if (!n) return;
    inorder(n->left);
    cout << n->nama << endl;
    inorder(n->right);
}

// ================= GRAPH =================
void tambahRelasi() {
    string a = inputString("Film1: ");
    string b = inputString("Film2: ");

    Movie* m1 = searchHash(a);
    Movie* m2 = searchHash(b);

    if (!m1 || !m2) return;

    if (m1->relatedCount < 5)
        m1->related[m1->relatedCount++] = m2;
    else
        cout << "Full\n";
}

void tampilRelasi() {
    string nama = inputString("Film: ");
    Movie* m = searchHash(nama);

    for (int i = 0; i < m->relatedCount; i++)
        cout << "- " << m->related[i]->nama << endl;
}

// ================= GENRE =================
void tampilByGenre() {
    for (int i = 0; i < genreCountGlobal; i++)
        cout << i+1 << ". " << genreList[i].nama << endl;

    int pilih = inputInt("Pilih: ", 1, genreCountGlobal) - 1;

    for (int i = 0; i < genreList[pilih].filmCount; i++)
        cout << genreList[pilih].films[i]->nama << endl;
}

// ================= MAIN =================
int main() {
    initHash();
    initGenre();

    int pilih;

    do {
        cout << "\n1.Tambah 2.Tampil 3.Sort 4.Cari 5.Undo\n";
        cout << "6.Queue 7.Proses 8.BST 9.Relasi 10.Lihat Relasi 11.Genre 0.Exit\n";

        pilih = inputInt(">> ", 0, 11);

        switch (pilih) {
            case 1: tambahFilm(); break;
            case 2: tampilFilm(); break;
            case 3: sortFilm(); break;
            case 4: cariFilm(); break;
            case 5: undoFilm(); break;
            case 6: tambahAntrian(); break;
            case 7: prosesAntrian(); break;
            case 8: inorder(root); break;
            case 9: tambahRelasi(); break;
            case 10: tampilRelasi(); break;
            case 11: tampilByGenre(); break;
        }

    } while (pilih != 0);
}