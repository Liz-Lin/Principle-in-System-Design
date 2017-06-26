#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#define STRMAX 40
typedef struct Song{
    char title[STRMAX];
    char artist[STRMAX];
    int year_published;
}Song;

#define SONGMAX 1024
Song music_library[SONGMAX];
int current_number_of_songs = 0;
char *file_name;
FILE *f;


void decide_file_name(int argc, char *argv[] )
{
    if(argc == 1)
        file_name = "myMusic";
    else
        file_name = argv[1]; 
}

void empty_file (FILE* f)
{
    if(f == NULL){
        printf("file not exit");
        exit(1);
    }
}


void open_file( char mode)
{
    if(mode == 'r' )
        f = fopen(file_name, "r");
    
    else if ( mode == 'w')
    {
        f = fopen(file_name, "w");
        empty_file(f);    
    }
    
}



void print_one_song (int i, bool is_index)
{
    if(is_index)
	printf("%d Title: %s , Artist: %s , Year Published: %d\n", i+1, music_library[i].title, music_library[i].artist, music_library[i].year_published);
    else
	printf("Title: %s , Artist: %s , Year Published: %d\n", music_library[i].title, music_library[i].artist, music_library[i].year_published);
}


void print_MusicLibrary()
{
    for(int i =0; i< current_number_of_songs; ++i)
        print_one_song(i,true);
    
}

void write_song (int i)
{
    fprintf(f, "%s\n", music_library[i].title);
    fprintf(f, "%s\n", music_library[i].artist);
    fprintf(f, "%d\n", music_library[i].year_published);
}

void write_song_2(Song song)
{
    fprintf(f, "Title: %s, Artist: %s, Year Published: %d\n", song.title, song.artist, song.year_published);
}

void store_MusicLibrary ()
{
    open_file('w');
    for(int i =0; i< current_number_of_songs; ++i)
        write_song(i);
}

void get_rid_of_new_line(char* to_change)
{
     char* n = strchr(to_change, '\n');
     if (n != NULL)
	*n = '\0';

}
void get_string_info(char* to_change, char* question)
{
    printf("%s", question);
    fgets(to_change, STRMAX, stdin);
    char* n = strchr(to_change, '\n');
    if(n != NULL)
        *n = '\0';
}

int get_year_info ()
{
    int year;
    printf("Year Published: ");
    scanf("%d", &year);
    return year;
}


Song get_song_info()
{
    Song song;
    get_string_info(song.title, "Title: ");
    get_string_info(song.artist,"Artist: " );
    song.year_published = get_year_info();
    return song;
}


char* to_lower (char* source, char* target)
{
    for(int i=0; source[i] != '\0'; ++i)
        target[i] = tolower(source[i]);
    return target;
}


int binary_search(char* name, int min, int max)
{
    if (max < min)
        return min;
    int mid = (max + min)/2;
    int result = strcasecmp (music_library[mid].title,name);
    return result >0?binary_search(name, min, mid -1) : result <0?binary_search(name, mid+1, max) : mid;
}

int find_index_of_song_with_name(char* name)
{
    return binary_search(name, 0, current_number_of_songs-1);
}

void crunch_down_from_index(int index)
{
    for(int i= current_number_of_songs-1; i >= index; --i)
        music_library[i+1] = music_library[i];
}

void crunch_up_from_index(int index)
{
    for(int i = index; i < current_number_of_songs; ++i)
        music_library[index] = music_library[index+1];
    
}
void add_song_to_MusicLibrary()
{
    Song song = get_song_info();
    int index =find_index_of_song_with_name(song.title);
    crunch_down_from_index(index);
    music_library[index]= song;
    ++current_number_of_songs;
}

bool check_existance_by_name (char* name, int index)
{
    if(strcmp(music_library[index].title, name) == 0)
        return true;
    else
        return false;
}
void remove_song_from_MusicLibrary_by_name (char* name)
{
    int index = find_index_of_song_with_name(name);
    if(check_existance_by_name(name,index)){
        crunch_up_from_index(index);
        --current_number_of_songs;
    }else
        printf("%s not exist, can't be removed\n", name);
}

void command_D ()
{
    char name[STRMAX];
    get_string_info(name, "Title: ");
    remove_song_from_MusicLibrary_by_name(name);
}

void look_up_song_with_name (char* name)
{
    int index = find_index_of_song_with_name(name);
    if(check_existance_by_name(name,index))
        print_one_song(index,false);
    else
        printf("%s not exist, can't be looked up\n", name);
}

void command_L ()
{
    char name[STRMAX];
    get_string_info(name, "Title: ");
    look_up_song_with_name(name);
}

void add_the_song_to_MusicLibrary(Song song)
{
    int index =find_index_of_song_with_name(song.title);
    crunch_down_from_index(index);
    music_library[index]= song;
    ++current_number_of_songs;
}

void read_all_songs_from_file()//excepttion 1 line =6 >5
{
	Song song;
	char ch;
	while(fgets(song.title, STRMAX, f) != NULL)
	{
	    get_rid_of_new_line(song.title);
            fgets(song.artist, STRMAX, f);
	    get_rid_of_new_line(song.artist);	    
	    fscanf(f, "%d", &song.year_published);
	    ch = fgetc(f);
            add_the_song_to_MusicLibrary(song);	
	}
}

void load_MusicLibrary()
{
	f = fopen(file_name, "r");
	if(f == NULL)
		return;
	read_all_songs_from_file();
	fclose(f);
}

char read_command()
{
    char command;
    scanf(" %c%*c", &command);
    return toupper(command);
}

bool evaluate_command(char command)
{
    switch (command)
    {
        case 'I':
            add_song_to_MusicLibrary();
            break;
        case 'P':
            print_MusicLibrary();
            break;
        case 'D':
            command_D();
            break;
        case 'L':
            command_L();
            break;
        case 'Q':
            store_MusicLibrary();
            fclose(f);
            return true;
        default:
            printf("Wrong Command. Repeat\n");
    }
    return false;
}


int main(int argc, char *argv[]){
    decide_file_name(argc, argv);
    load_MusicLibrary();
    char command;
    do{
        command = read_command();
    }while(evaluate_command(command) != true);
    
    return 0;
}


