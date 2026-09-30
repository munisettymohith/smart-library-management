typedef struct book
{
    int id;
    char title[100];
    char author[100];
    char cat[50];
    int avail;
    int count;
    struct book *next;
} book;

typedef struct hnode
{
    int bid;
    int back;
    struct hnode *next;
} hnode;

typedef struct stu
{
    int id;
    char name[100];
    int count;
    hnode *hist;
    struct stu *next;
} stu;

extern book *head;

book *find_book(int id);
void add_book(void);
void show_books(void);
void edit_book(void);
void del_book(void);
void by_id(void);
void find_text(int field);
void sort_books(int type);
void put_book(book *b);
void bin_title(void);

void add_stu(void);
void show_stu(void);
stu *find_stu(int id);
void edit_stu(void);
void del_stu(void);
void hist_add(int sid, int bid);
void hist_back(int sid, int bid);
void show_mine(void);
int count_stu(void);
int count_busy(void);
stu *first_stu(void);
void load_stu(int id, const char *name);
void load_hist(int sid, int bid, int back);

void issue_book(void);
void return_book(void);
void show_queue(void);
void logs_save(const char *path);
void logs_load(const char *path);
float fine_of(int sid);

float calc_fine(int days);
void show_pop(void);
void stats(void);
void test_fine(void);

void recommend(void);

void save_all(void);
void load_all(void);
