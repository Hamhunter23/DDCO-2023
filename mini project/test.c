#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>

#define MAX_TITLE_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define MAX_LINE_LENGTH 256
#define MAX_PLAYLIST_NAME_LENGTH 50

// Structure to hold information about a song
typedef struct Song {
    char title[MAX_TITLE_LENGTH];
    char filename[MAX_FILENAME_LENGTH];
    struct Song *next;
} Song;

// Structure to manage the music player
typedef struct {
    Song *head;
    Song *tail;
    Song *currentSong; // Pointer to the current playing song (NULL if none)
    BOOL isPaused;     // Flag to track pause state
} MusicPlayer;

// Function to add a song to the playlist
void addSong(MusicPlayer *player, const char *title, const char *filename) {
    Song *newSong = (Song *)malloc(sizeof(Song));
    strcpy(newSong->title, title);
    strcpy(newSong->filename, filename);
    newSong->next = NULL;

    if (player->head == NULL) {
        player->head = newSong;
        player->tail = newSong;
    } else {
        player->tail->next = newSong;
        player->tail = newSong;
    }
}

// Function to display the playlist
void displayPlaylist(const MusicPlayer *player) {
    printf("Current Playlist:\n");
    int i = 1;
    for (Song *current = player->head; current != NULL; current = current->next) {
        printf("%d. %s\n", i++, current->title);
    }
}

// Function to play a song
void playSong(const char *filename, BOOL *isPaused) {
    printf("Playing: %s\n", filename);
    if (*isPaused) {
        PlaySound(NULL, 0, 0); // Stop the currently playing sound (if paused)
        *isPaused = FALSE;
    }
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}

// Function to pause the current song
void pauseSong(BOOL *isPaused) {
    printf("Pausing...\n");
    *isPaused = TRUE;
    PlaySound(NULL, 0, 0); // Stop the currently playing sound
}

// Function to resume playback
void resumeSong(const char *filename, BOOL *isPaused) {
    printf("Resuming...\n");
    *isPaused = FALSE;
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}

// Rest of your code remains the same ...
// Function to read songs from a given playlist CSV file and populate the playlist
void readSongsFromPlaylistCSV(MusicPlayer *player, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    // Skip the header line
    fgets(line, MAX_LINE_LENGTH, file);

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char title[MAX_TITLE_LENGTH];
        char filename[MAX_FILENAME_LENGTH];

        if (sscanf(line, "%[^,],%s", title, filename) == 2) {
            addSong(player, title, filename);
        } else {
            printf("Invalid line format: %s\n", line);
        }
    }

    fclose(file);
}

// Function to create a new playlist from selected songs
void createNewPlaylist(MusicPlayer *player) {
    char newPlaylistName[MAX_PLAYLIST_NAME_LENGTH];
    printf("Enter the name for the new playlist file (without extension): ");
    scanf("%s", newPlaylistName);

    char newPlaylistFilename[MAX_FILENAME_LENGTH];
    sprintf(newPlaylistFilename, "%s.csv", newPlaylistName);

    FILE *newPlaylistFile = fopen(newPlaylistFilename, "w");
    if (newPlaylistFile == NULL) {
        printf("Error creating the new playlist file.\n");
        return;
    }

    fprintf(newPlaylistFile, "Song Title,Filename\n");
    int i = 1;
    for (Song *current = player->head; current != NULL; current = current->next) {
        printf("Include '%s' in the new playlist? (1 for Yes / 0 for No): ", current->title);
        int include;
        scanf("%d", &include);

        if (include == 1) {
            fprintf(newPlaylistFile, "%s,%s\n", current->title, current->filename);
        }
        i++;
    }

    fclose(newPlaylistFile);
    printf("New playlist '%s' created successfully.\n", newPlaylistFilename);
}

// Function to play the next song in the playlist
void playNextSong(MusicPlayer *player) {
    if (player->currentSong != NULL) {
        player->currentSong = player->currentSong->next;
        if (player->currentSong != NULL) {
            printf("Playing next song: %s\n", player->currentSong->title);
            playSong(player->currentSong->filename, &(player->isPaused));
        } else {
            printf("End of playlist.\n");
        }
    } else {
        printf("No song is currently playing.\n");
    }
}

// Function to play the previous song in the playlist
void playPreviousSong(MusicPlayer *player) {
    if (player->currentSong != NULL) {
        Song *previousSong = NULL;
        for (Song *current = player->head; current != NULL; current = current->next) {
            if (current == player->currentSong) {
                player->currentSong = previousSong;
                if (player->currentSong != NULL) {
                    printf("Playing previous song: %s\n", player->currentSong->title);
                    playSong(player->currentSong->filename, &(player->isPaused));
                } else {
                    printf("Beginning of playlist.\n");
                }
                break;
            }
            previousSong = current;
        }
    } else {
        printf("No song is currently playing.\n");
    }
}

// Rest of your code remains the same ...

int main() {
    MusicPlayer player;
    player.head = NULL;
    player.tail = NULL;
    player.currentSong = NULL; // No song playing initially
    player.isPaused = FALSE;

    // Read songs from default playlist CSV file
    readSongsFromPlaylistCSV(&player, "playlist.csv");

    while (1) {
        int choice;
        printf("\nMenu:\n1. Display Playlist\n2. Play Song\n3. Create New Playlist\n4. Play Next Song\n5. Play Previous Song\n6. Play Song from New Playlist\n7. Display New Playlist\n8. Pause\n9. Resume\n10. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayPlaylist(&player);
                break;
            case 2: {
                int songIndex;
                printf("Enter song number to play: ");
                scanf("%d", &songIndex);
                if (songIndex > 0) {
                    Song *current = player.head;
                    for (int i = 1; i < songIndex && current != NULL; i++) {
                        current = current->next;
                    }
                    if (current != NULL) {
                        player.currentSong = current;
                        playSong(player.currentSong->filename, &(player.isPaused));
                    } else {
                        printf("Invalid song number.\n");
                    }
                } else {
                    printf("Invalid song number.\n");
                }
                break;
            }
            case 3:
                createNewPlaylist(&player);
                break;
            case 4:
                playNextSong(&player);
                break;
            case 5:
                playPreviousSong(&player);
                break;
            case 6: {
                char newPlaylistFilename[MAX_FILENAME_LENGTH];
                printf("Enter the name of the new playlist to play from (without extension): ");
                scanf("%s", newPlaylistFilename);
                strcat(newPlaylistFilename, ".csv");
                MusicPlayer newPlayer;
                newPlayer.head = NULL;
                newPlayer.tail = NULL;
                newPlayer.currentSong = NULL;
                newPlayer.isPaused = FALSE;
                readSongsFromPlaylistCSV(&newPlayer, newPlaylistFilename);

                displayPlaylist(&newPlayer);

                int songIndex;
                printf("Enter song number to play from the new playlist: ");
                scanf("%d", &songIndex);
                if (songIndex > 0) {
                    Song *current = newPlayer.head;
                    for (int i = 1; i < songIndex && current != NULL; i++) {
                        current = current->next;
                    }
                    if (current != NULL) {
                        player = newPlayer;
                        player.currentSong = current;
                        playSong(player.currentSong->filename, &(player.isPaused));
                    } else {
                        printf("Invalid song number.\n");
                    }
                } else {
                    printf("Invalid song number.\n");
                }
                break;
            }
            case 7:
                // Display songs from the new playlist
                readSongsFromPlaylistCSV(&player, "new_playlist.csv");
                displayPlaylist(&player);
                break;
            case 8:
                if (player.currentSong != NULL) {
                    pauseSong(&(player.isPaused));
                } else {
                    printf("No song is currently playing.\n");
                }
                break;
            case 9:
                if (player.isPaused) {
                    resumeSong(player.currentSong->filename, &(player.isPaused));
                } else {
                    printf("No paused song to resume.\n");
                }
                break;
            case 10:
                printf("Exiting the music player.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}