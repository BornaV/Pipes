#include <stdio.h>
#include <stdlib.h>
#include "framework.h"
#define key "ESPipes"

// ----------------------------------------------------------------------------
// Rotates the pipe depending on the rotation direction
//
// @param value                   value of the pipe before rotation
// @param rotation                what direction is the pipe turning
//
// @return                        returns the data of the pipe after rotation
//
uint8_t rotate_pipe_without_conflict(uint8_t value, uint8_t rotation);

// ----------------------------------------------------------------------------
// Rotates the pipe depending on the rotation direction
//
// @param value                   value of the pipe before rotation
// @param rotation                what direction is the pipe turning
//
// @return                        returns the data of the pipe after rotation
//
uint8_t rotate_pipe_with_conflict(uint8_t value, uint8_t rotation);

// ----------------------------------------------------------------------------
// Determens if there would be lost data if bitshifting is used
//
// @param value     value of the pipe before rotation
// @param rotation   what direction is the pipe turning
//
bool could_conflict_occur(uint8_t value, uint8_t rotation);

// ----------------------------------------------------------------------------
// Removes all connections on the pipe
//
// @param value                   value of pipe
//
// @return                        returns the pipe without connections
//
uint8_t remove_pipe_connections(uint8_t value);

// ----------------------------------------------------------------------------
// Determens if the input is correct
//
// @param user_input              the input given by the user
// @param cmmd                    command that was given by the user
// @param direction               what direction was entered
// @param row                     inputed row of the map field
// @param col                     inputed collumn of the map field
// @param height                  height of the map field
// @param width                   width of the map field
// @param location_startPipe      cordinates of the start Pipe
// @param location_endPipe        cordinates of the end Pipe
//
// @return                        returns value dependant on the user input
//
bool is_input_valid(char* user_input, uint8_t cmmd,uint8_t direction,uint8_t row,uint8_t col,uint8_t height,uint8_t width, uint8_t* location_startPipe, uint8_t* location_endPipe);

// ----------------------------------------------------------------------------
// Reads the input file and parse-s the data into usable forms
//
// @param file                    file that will be read
// @param magword                 magical word, or Key to determen if the file is valid
// @param width                   the width of the map
// @param height                  the height of the map
// @param startPipe               location of the start pipe
// @param endPipe                 location of the end pipe
// @param submissions             amount of submissions contained in the file
// @param size                    the amount of characters in the file
//
void get_map_properties(const char** file, char* magword, uint8_t* width, uint8_t* height, uint8_t* startPipe, uint8_t* endPipe,uint8_t* submissions, uint8_t* size);

// ----------------------------------------------------------------------------
// Copies the map data from the file to the map_data 
//
// @param file                    file that will be read
// @param map_data                variable that contains the map data
// @param submissions             amount of submissions contained in the file
// @param size                    the amount of characters in the file
//
void get_map_data(const char** file, uint8_t* map_data,uint8_t submissions, uint8_t size);

// ----------------------------------------------------------------------------
// Converts the map into usuable 2d array for editing 
//
// @param map_data                variable that contains the map data
// @param map_array               variable that contains the map data
// @param height                  the width of the map
// @param width                   the hight of the map
//
void convert_map_to_2d_array(uint8_t* map_data, uint8_t **map_array, uint8_t height , uint8_t width);

// ----------------------------------------------------------------------------
// Checks if there is a possible connections inbetween two pipes
//
// @param open_connections        is the connection even possible
// @param value                   data of the pipe that is being analised
//
void check_possible_connection(uint8_t* open_connections, uint8_t value);

// ----------------------------------------------------------------------------
// Connects the pipes that are given
//
// @param map_data                variable that contains the map data
// @param courrent_field          location of the currently selected pipe
// @param above_field             pipe above the currently selected one
// @param behind_field            pipe behind the currently selected one
// @param bellow_field            pipe below the currently selected one
// @param next_field              pipe after the currently selected one
//
void connect_pipes(uint8_t *map_data,uint8_t* courrent_field, uint8_t* above_field, uint8_t* behind_field, uint8_t* bellow_field, uint8_t* next_field);

// ----------------------------------------------------------------------------
// Goes trough the whole map and rebuilds the connections inbetween pipes
//
// @param map_data                variable that contains the map data
// @param height                  the width of the map
// @param width                   the height of the map
//
void rebuild_every_connection(uint8_t* map_data, uint8_t height, uint8_t width);

// ----------------------------------------------------------------------------
// Updates the map data to the courrent one
//
// @param map_data                variable that contains the map data
// @param height                  the width of the map
// @param width                   the height of the map
// @param row                     inputed row of the map field
// @param col                     inputed collumn of the map field
// @param new_value               the value of the new pipe
//
void edit_map_field(uint8_t *map_data, uint8_t height , uint8_t width, uint8_t row, uint8_t col, uint8_t new_value);

// ----------------------------------------------------------------------------
// Gets the data of the desired pipe
//
// @param map_data                variable that contains the map data
// @param height                  the width of the map
// @param width                   the height of the map
// @param row                     inputed row of the map field
// @param col                     inputed collumn of the map field
//
// @return                        returns the data of the desired pipe
//
uint8_t get_map_value(uint8_t *map_data, uint8_t height , uint8_t width, uint8_t row, uint8_t col );

// ----------------------------------------------------------------------------
// Updates the submissions_result variable with the new highscore
//
// @param submissions_result      submissions currently int the config file
// @param username_score          score of the user
// @param amount_of_submissions   the maximum supported ammount of submissions
//
void update_highscore(char *submissions_result, char* username_score, uint8_t amount_of_submissions);

// ----------------------------------------------------------------------------
// Updates the config file with the new highscore
//
// @param file                    file that will be read
// @param submissions             location where the submissions are stored
// @param score                   the new submission data
//
void update_existing_file(const char** file, uint8_t submissions, char* score);

// ----------------------------------------------------------------------------
// Gets the scores and the names of previous submissions
//
// @param file                    file that will be read
// @param submissions             location where the submissions are stored
// @param score                   location where the data will be stored
//
void get_highscore_and_name(const char** file, uint8_t submissions, char* score);

// ----------------------------------------------------------------------------
// Makes sure that the text is uppercase
//
// @param text                    text to edit
//
void toUpper(char *text);




// ----------------------------------------------------------------------------
uint8_t rotate_pipe_without_conflict(uint8_t value, uint8_t rotation)
{
  if (rotation == 3)
  {
    value = value>>2;
    return value;
  }
  if (rotation == 1)
  {
    value = value<<2;
    return value;
  }
  return value;
}

// ----------------------------------------------------------------------------
uint8_t rotate_pipe_with_conflict(uint8_t value, uint8_t rotation)
{
  if (rotation == 3)
  {
    value = value>>2;
    value += 128;
    return value;
  }
  if (rotation == 1)
  {
    value = value<<2;
    value += 2;
    return value;
  }
  return value;
}

// ----------------------------------------------------------------------------
bool could_conflict_occur(uint8_t value, uint8_t rotation)
{
  if (((value & 128 ) == 128) && rotation == 1)
  {
    //printf("\nConflict 0\n\n");
    return true;
  }
  if (((value & 2 ) == 2) && rotation == 3)
  {
    //printf("\nConflict 1\n\n");
    return true;
  }
  return false;
  
}

// ----------------------------------------------------------------------------
uint8_t remove_pipe_connections(uint8_t value)
{
  uint8_t value_without_connections = value;
  if ((value & 1 ) == 1)
  {
    value_without_connections -= 1;
  }
  if ((value & 4 ) == 4)
  {
    value_without_connections -= 4;
  }
  if ((value & 16 ) == 16)
  {
    value_without_connections -= 16;
  }
  if ((value & 64 ) == 64)
  {
    value_without_connections -= 64;
  }
  return value_without_connections;
}

// ----------------------------------------------------------------------------
bool is_input_valid(char* user_input, uint8_t cmmd,uint8_t direction,uint8_t row,uint8_t col,uint8_t height,uint8_t width, uint8_t* location_startPipe, uint8_t* location_endPipe)
{
  if (cmmd == 1)
  {
    if (direction == 3 || direction == 1)
    {
      if (row == location_startPipe[0]+1&& col == location_startPipe[1]+1 )
      {
        printf("%s", ERROR_ROTATE_INVALID);
        return 0;
      }
      if (row == location_endPipe[0]+1 && col == location_endPipe[1]+1 )
      {
        printf("%s", ERROR_ROTATE_INVALID);
        return 0;
      }
      if (row > height  || col > width)
      {
        printf("%s", USAGE_COMMAND_ROTATE);
        return 0;
      }
      if (row == 0 || col == 0)
      {
        printf("%s", USAGE_COMMAND_ROTATE);
        return 0;
      }
      
    }else
    {
      printf("%s", USAGE_COMMAND_ROTATE);
      return 0;
    }
   
    
    return 1;
  }
  else if (cmmd == 2)
  {
    return 1;
  }
  else if(cmmd < 1 || cmmd > 4)
  {
    printf("Error: Unknown command: %s\n", user_input);
     
    return 0;
  }
  
  

  return 1;
}

// ----------------------------------------------------------------------------
void get_map_properties(const char** file, char* magword, uint8_t* width, uint8_t* height, uint8_t* startPipe, uint8_t* endPipe,uint8_t* submissions, uint8_t* size)
{
  char buffer[1];
  FILE *ptr;
  ptr = fopen(file[1],"r");
  fseek(ptr, 0, SEEK_SET);
  int i = 0;
  while(!feof(ptr))
  {
      fread(buffer, sizeof(char), 1, ptr);
      //printf("%02x  ", buffer[0]);
      if (i <= 6)
      {
      *magword = buffer[0];
      magword++;
      }
      else if (i == 7)
      {
      *width = buffer[0];
      }
      else if (i == 8)
      {
      *height = buffer[0];
      }
      else if (i == 9 || i == 10)
      {
      *startPipe = buffer[0];
      startPipe++;
      }
      else if (i == 11 || i == 12)
      {
      *endPipe = buffer[0];
      endPipe++;
      }
      else if (i == 13 )
      {
      *submissions = buffer[0];
      }
      
      i++;
  }
  *size = i;
  fclose(ptr);
}

// ----------------------------------------------------------------------------
void get_map_data(const char** file, uint8_t* map_data,uint8_t submissions, uint8_t size)
{
  char buffer[1];
  FILE *ptr;
  ptr = fopen(file[1],"r");
  fseek(ptr, (14 + (submissions * 4)), SEEK_SET);
  
  for (int i = 0; i < (size - (14 + (submissions * 4))); i++)
  {
    fread(buffer, sizeof(char), 1, ptr);
    *map_data = buffer[0];
    map_data++;
  }

  fclose(ptr);
}

// ----------------------------------------------------------------------------
void convert_map_to_2d_array(uint8_t* map_data, uint8_t **map_array, uint8_t height , uint8_t width)
{
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      *((map_array+i)+j) = &map_data[0];
      map_data++;
    }
  }
}

// ----------------------------------------------------------------------------
void check_possible_connection(uint8_t* open_connections, uint8_t value)
{
  if ((value & 128 ) == 128)
  {
    open_connections[0] = 1;//top open
  }else
  {
    open_connections[0] = 0;//top open
  }
  if ((value & 32 ) == 32)
  {
    open_connections[1] = 1;//left open 
  }else
  {
    open_connections[1] = 0;//left open 
  }
  if ((value & 8 ) == 8)
  {
    open_connections[2] = 1;//bottom open
  }else
  {
    open_connections[2] = 0;//bottom open
  }
  if ((value & 2 ) == 2)
  {
    open_connections[3] = 1;//right open
  }else
  {
    open_connections[3] = 0;//right open
  }
}

// ----------------------------------------------------------------------------
void connect_pipes(uint8_t *map_data,uint8_t* courrent_field, uint8_t* above_field, uint8_t* behind_field, uint8_t* bellow_field, uint8_t* next_field)
{
  map_data[0] = remove_pipe_connections(map_data[0]);
  if(courrent_field[0] == 1 && above_field[2] == 1)
  {
    map_data[0] += 64; 
  }
  if(courrent_field[1] == 1 && behind_field[3] == 1)
  {
    map_data[0] += 16; 
  }
  if(courrent_field[2] == 1 && bellow_field[0] == 1)
  {
    map_data[0] += 4; 
  }
  if(courrent_field[3] == 1 && next_field[1] == 1)
  {
    map_data[0] += 1; 
  }
}

// ----------------------------------------------------------------------------
void rebuild_every_connection(uint8_t* map_data, uint8_t height, uint8_t width)
{
  uint8_t courrent_field[4];
  uint8_t above_field[4];
  uint8_t behind_field[4];
  uint8_t bellow_field[4];
  uint8_t next_field[4];
  for (int i = 1; i <= height; i++)
  {
    for (int j = 1; j <=  width; j++)
    {
      check_possible_connection(&courrent_field[0], map_data[0]);
      check_possible_connection(&above_field[0], map_data[0-width]);
      check_possible_connection(&behind_field[0], map_data[0-1]);
      check_possible_connection(&next_field[0], map_data[1] );
      check_possible_connection(&bellow_field[0], map_data[width]);
      connect_pipes(&map_data[0], &courrent_field[0], &above_field[0], &behind_field[0], &bellow_field[0], &next_field[0]);
      map_data++;
    }
  }
}

// ----------------------------------------------------------------------------
void edit_map_field(uint8_t *map_data, uint8_t height , uint8_t width, uint8_t row, uint8_t col, uint8_t new_value)
{
  for (int i = 1; i <= height; i++)
  {
    for (int j = 1; j <= width; j++)
    {
      if (row  == i && col == j)
      {
        map_data[0] = new_value;
      }
      map_data++;
    }
  }
}

// ----------------------------------------------------------------------------
uint8_t get_map_value(uint8_t *map_data, uint8_t height , uint8_t width, uint8_t row, uint8_t col )
{ 
  uint8_t temp = 0; 
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (row == i+1 && col == j+1)
      {
        temp = map_data[0];
        return temp;
      }
      map_data++;
    }
  }
  return temp;
}

// ----------------------------------------------------------------------------
void update_highscore(char *submissions_result, char* username_score, uint8_t amount_of_submissions)
{
  uint8_t username_entered = 0;
  for (int i = 0; (amount_of_submissions * 4) > i; i = i+4)
  {
    char temp[4];
    temp[0] = submissions_result[i];
    temp[1] = submissions_result[i+1];
    temp[2] = submissions_result[i+2];
    temp[3] = submissions_result[i+3];
    if (temp[0] > username_score[0] && username_entered == 0)
    {
      submissions_result[i] = username_score[0];
      submissions_result[i+1] = username_score[1];
      submissions_result[i+2] = username_score[2];
      submissions_result[i+3] = username_score[3];
      username_entered = 1;
    }
    if (username_entered == 1)
    {
      username_score[0] = temp[0];
      username_score[1] = temp[1];
      username_score[2] = temp[2];
      username_score[3] = temp[3];
      username_entered = 0;
    }
  }
}

// ----------------------------------------------------------------------------
void update_existing_file(const char** file, uint8_t submissions, char* score)
{
  FILE *ptr;
  ptr = fopen(file[1],"r+");
  fseek(ptr, 14, SEEK_SET);
  for (int i = 0; i < (submissions * 4); i++)
  {
    fputc(score[0], ptr);
    score++;
  }
  fclose(ptr);
}

// ----------------------------------------------------------------------------
void get_highscore_and_name(const char** file, uint8_t submissions, char* score)
{
  char buffer[1] = {'\0'};
  FILE *ptr;
  ptr = fopen(file[1],"r");
  fseek(ptr, 14, SEEK_SET);
  for (int i = 0; i < (submissions * 4); i++)
  {
    fread(buffer, sizeof(char), 1, ptr);
    *score = buffer[0];
    score++;
  }
  fclose(ptr);
}

// ----------------------------------------------------------------------------
void toUpper(char *text) 
{
  for (int i = 0; i <= 25; i++)
  {
    if (*text != ' ' && *text >= 97 && *text <= 122)
    {
      *text = *text - 32;
    }
    if (*text == '\0')
    {
     break;
    }
    text++;
  }
}

int main(int argc, char const **argv)
{
  char magic_word[7];
  uint8_t width = '\0';
  uint8_t height = '\0';
  uint8_t location_startPipe[2];
  uint8_t location_endPipe[2];
  uint8_t amount_of_submissions = '\0';
  char *submissions_result;
  uint8_t size;
  uint8_t map_data[50];
  uint8_t **map_array;
  char *user_input;
  Command cmmd;
  size_t direction;
  uint8_t row;
  uint8_t col;
  uint8_t value_of_sector;
  uint8_t value_after_rotation;
  int g = 1;
  if (argc != 2)
  {
    printf("%s",USAGE_APPLICATION);
    return 1;
  }
  
  get_map_properties(&argv[0], &magic_word[0], &width, &height, &location_startPipe[0], &location_endPipe[0],&amount_of_submissions, &size);
  
  for (int i = 0; i < 7; i++)
  {
    if (key[i] != magic_word[i])
    {
      printf("Error: Invalid file: %s\n", argv[1]);
      return 3;
    }
  }
  
  get_map_data(&argv[0], &map_data[0], amount_of_submissions, size);
  map_array = (uint8_t**) malloc(sizeof(uint8_t*) * height);
  for (int i = 0; i < height; i++)
  {
    *(map_array + i) = (uint8_t*) malloc(sizeof(uint8_t)*width);

  }
  while (g >= 1)
  {
    convert_map_to_2d_array(&map_data[0], &map_array[0], height, width );
    printMap(map_array, width, height, location_startPipe, location_endPipe);
    bool is_finished = arePipesConnected(map_array, width, height, location_startPipe, location_endPipe);
    if (is_finished == true)
    {
      printf("%s",INFO_PUZZLE_SOLVED);
      printf("Score: %d\n",g - 1); 
      submissions_result = (char* )malloc((amount_of_submissions * 4) * sizeof(char));
      get_highscore_and_name(&argv[0], amount_of_submissions, &submissions_result[0]);
      char username[4];
      for (int i = 0; i < 5; i++)
      {
        user_input[i]= '\0';map_array = (uint8_t**) malloc(sizeof(uint8_t*) * height);
        for (int i = 0; i < height; i++)
        {
          *(map_array) = (uint8_t*) malloc(sizeof(uint8_t)*width);
        }
        
        for (int i = 0; i <= amount_of_submissions; i++)
        {
          if(submissions_result[(i*4)-4] > (g-1))
          {
            printf("%s",INFO_BEAT_HIGHSCORE);
            bool is_valid = false;
            do
            {
              printf("%s",INPUT_NAME);
              scanf("%s",username);
              if(username[1] == '\0' || username[2] == '\0' || username[3] != '\0')
              {
                printf("%s",ERROR_NAME_LENGTH);
                is_valid = false;
              }
              else
              {
                is_valid = true;
              }
              for (int i = 0; i < 3; i++)           
              {
                if ((username[i] >= 97 && username[i] <= 122) || (username[i] >= 65 && username[i] <= 90))
                {

                }
                else
                {
                  printf("%s",ERROR_NAME_ALPHABETIC);
                  is_valid = false;
                }
                
              }
              
              
            } while (is_valid == false);
            toUpper(username); 
            char username_score[4]= {g -1 ,username[0],username[1],username[2]};
            update_highscore(&submissions_result[0], &username_score[0], amount_of_submissions);
            update_existing_file(&argv[0],amount_of_submissions, &submissions_result[0]);
            break;
          }      
        }
        printf("%s",INFO_HIGHSCORE_HEADER);
        for (int i = 0; i < (amount_of_submissions * 4); i = i + 4)
        {
          printf("   %c", submissions_result[i+1]);
          printf("%c", submissions_result[i+2]);
          printf("%c ", submissions_result[i+3]);
          printf("%d\n", submissions_result[i]);
        }
        free(submissions_result);
        return 0;
      }
    }
    int valid_input = 0;
    do
    {
      direction = 0;
      row = 0;
      col = 0;
      printf("%d > ", g);
      user_input = malloc(sizeof(char)* 50);
      user_input = getLine();
      parseCommand(&user_input[0], &cmmd, &direction, &row, &col);
      valid_input = is_input_valid(&user_input[0], cmmd, direction, row, col, height, width, &location_startPipe[0], &location_endPipe[0]);
      free(user_input);
      if (cmmd == 2)
      {
        printf("%s",HELP_TEXT);
      }
      if (cmmd == 3)
      {
        free(map_array);

        exit(0);
      }
      if (cmmd == 4)
      {
        get_map_data(&argv[0], &map_data[0], amount_of_submissions, size);
        g = 0;
      }
    } while ( valid_input == 0);
    g++;
    if (direction == 3)   //flipping the directions 
    {
      direction = 1;
    }else
    {
      direction = 3;
    }
    value_of_sector = get_map_value(&map_data[0], height , width, row, col);
    if (value_of_sector > 0 && value_of_sector < 255)
    {
      value_after_rotation = remove_pipe_connections(value_of_sector);    
      (could_conflict_occur(value_after_rotation, direction)) ? (value_after_rotation = rotate_pipe_with_conflict(value_after_rotation, direction)) :
        (value_after_rotation = rotate_pipe_without_conflict(value_after_rotation, direction));
      edit_map_field(&map_data[0], height , width, row, col, value_after_rotation);
    }
    rebuild_every_connection(&map_data[0],height, width);
  }
}


