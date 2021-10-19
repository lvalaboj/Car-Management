#include "carmanagement.h"

#include <stdio.h>
#include <ctype.h>

int g_cars_count = 0;
char g_vin[MAX_RECORDS][VIN_LEN] = {{'\0'}};
int g_price[MAX_RECORDS] = {0};
float g_rating[MAX_RECORDS][NUM_RATING_COLS] = {{0}};
char g_basic_info[MAX_RECORDS][NUM_BASIC_COLS][MAX_INFO_LEN] = {{{'\0'}}};

/*
 * This function reads a file and organizes the information read into
 * different arrays. The parameter, in_file, is the file that contains
 * this information that is organized into arrays.
 */

int read_tables(char *in_file) {
  FILE *read = NULL;
  int rec_count = 0;
  char vin[1000] = {""};
  char make[1000] = {""};
  char model[1000] = {""};
  char color[10000] = {""};
  char dealer_name[1000] = {""};
  float performance = 0.0;
  float comfort = 0.0;
  float reliability = 0.0;
  int price = 0;
  int status = 0;
  int vin_count = 0;
  int g_vin_count = 0;
  int g_basic_count = 0;
  int g_rate_count = 0;
  int g_price_count = 0;

  read = fopen(in_file, "r");
  if (read != NULL) {
    rec_count = 0;
    do {
      vin_count = 0;
      status = fscanf(read, "%18[^;];%50[^;];%50[^;];%50[^;];%50[^;];%f;%f;%f;"
      "%d\n", vin, make, model, color, dealer_name, &performance, &comfort,
      &reliability, &price);
      if (status == -1) {
        break;
      }

      for (int i = 0; i < VIN_LEN; i++) {
        if (vin[i] != 0) {
          if ((isupper(vin[i])) || (isdigit(vin[i]))) {
            if ((vin[i] != 73) && (vin[i] != 79) && (vin[i] != 81)) {
              vin_count++;
            }
          }
        }
      }

      for (int i = 0; i < VIN_LEN; i++) {
        g_vin[g_vin_count][i] = vin[i];
      }
      g_vin_count++;

      for (int k = 0; k < 50; k++) {
        g_basic_info[g_basic_count][0][k] = make[k];
      }
      for (int j = 0; j < 50; j++) {
        g_basic_info[g_basic_count][1][j] = model[j];
      }
      for (int l = 0; l < 50; l++) {
        g_basic_info[g_basic_count][2][l] = color[l];
      }
      for (int m = 0; m < 50; m++) {
        g_basic_info[g_basic_count][3][m] = dealer_name[m];
      }

      for (int i = 0; i < 50; i++) {
        make[i] = '\0';
        model[i] = '\0';
        color[i] = '\0';
        dealer_name[i] = '\0';
      }

      g_basic_count++;

      g_rating[g_rate_count][0] = performance;
      g_rating[g_rate_count][1] = comfort;
      g_rating[g_rate_count][2] = reliability;
      g_rate_count++;

      g_price[g_price_count] = price;
      g_price_count++;

      if (vin_count != 17) {
        fclose(read);
        read = NULL;
        return RECORD_ERROR;
      }
      else if (status != 9) {
        fclose(read);
        read = NULL;
        return RECORD_ERROR;
      }
      rec_count++;
    } while (status != -1);
    g_cars_count = rec_count;

    if (rec_count == 0) {
      fclose(read);
      read = NULL;
      return NO_DATA_POINTS;
    }
    else if (rec_count >= MAX_RECORDS) {
      fclose(read);
      read = NULL;
      return OUT_OF_BOUNDS;
    }

    fclose(read);
    read = NULL;
    return rec_count;

  }
  return FILE_READ_ERROR;
} /* read_tables() */

/*
 * This function looks through the arrays that were created
 * through the information in a certain file, and conts the
 * number of cars given a certain attribute. The first
 * parameter, given_make, is the attribute that the function
 * looks for while counting the cars.
 */

int count_cars_by_make(char *given_make) {
  int same_make_count = 0;
  int counter = 0;
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {

      if (given_make[j] == g_basic_info[i][0][j]) {
        same_make_count++;
      }
    }
    if (same_make_count == 50) {
      counter++;
    }
    same_make_count = 0;

  }

  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      g_basic_info[i][0][j] = '\0';
      given_make[j] = '\0';
    }
  }
  if (counter == 0) {
    return NO_SUCH_DATA;
  }
  return counter;
} /* count_cars_by_make() */

/*
 * This function looks through the entire price array
 * to find the index of the cheapest car. This function
 * has no parameters.
 */

int find_cheapest_car() {
  int min = 0;
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  for (int i = 1; i < 50; i++) {
    if (g_price[i] != '\0') {
      if (g_price[min] > g_price[i]) {
        min = i;
      }
    }
  }
  return min;
} /* find_cheapest_car() */

/* This function returns the index of the cheapest car
 * given a certain attribute. The first parameter,
 * given_color, is the attribute that the function looks
 * for to find the cheapest car.
 */

int find_cheapest_car_by_color(char *given_color) {
  int color_count = 0;
  int color_arr[50];
  int color_index = 0;
  int min = 0;
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      if (given_color[j] == g_basic_info[i][2][j]) {
        color_count++;
      }
    }
    if (color_count == 50) {
      color_arr[color_index] = i;
      color_index++;
    }
    color_count = 0;

  }

  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      g_basic_info[i][2][j] = '\0';
      given_color[j] = '\0';
    }
  }
  if (color_index == 0) {
    return NO_SUCH_DATA;
  }

  int index = 0;
  min = color_arr[0];
  for (int i = 0; i < 50; i++) {
    if (index < color_index) {
      if (i == color_arr[index]) {
        if (g_price[i] != '\0') {
          if (g_price[i] < g_price[min]) {
            min = i;
          }
        }
        index++;
      }
    }
  }

  return min;
} /* find_cheapest_car_by_color() */

/* This function counts cars given a certain attribute and
 * a range of prices. The first parameter,given_dealer, is the
 * attribute thats looked for while counting the cars. The second
 * parameter, min_price, is where the price range starts. The third
 * parameter, max_price, is where the price range ends.
 */

int count_cars_by_dealer(char *given_dealer, int min_price, int max_price) {
  int same_dealer_count = 0;
  int dealer_arr[50];
  int dealer_index = 0;
  int counter = 0;

  if ((min_price < 0) || (max_price < 0)) {
    return INVALID_PRICE;
  }

  if (min_price > max_price) {
    return INVALID_PRICE;
  }
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      if ((given_dealer[j] == g_basic_info[i][3][j])) {
        same_dealer_count++;
      }
    }
    if (same_dealer_count == 50) {
      dealer_arr[dealer_index] = i;
      dealer_index++;
    }
    same_dealer_count = 0;

  }

  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      g_basic_info[i][3][j] = '\0';
      given_dealer[j] = '\0';
    }
  }


  if (dealer_index == 0) {
    return NO_SUCH_DATA;
  }

  int index = 0;
  for (int i = 0; i < 50; i++) {
    if (index < dealer_index) {
      if (i == dealer_arr[index]) {
        if (g_price[i] != '\0') {
          if ((g_price[i] >= min_price) && (g_price[i] <= max_price)) {
            counter++;
          }
          index++;
        }
      }
    }
  }

  for (int i = 0; i < 50; i++) {
    g_price[i] = '\0';
  }


  return counter;
} /* count_cars_by_dealer() */

/* This function returns the average of all the ratings from an array.
 * The first parameter, given_vin, is the vin that the average is
 * calculated for.
 */

float calculate_avg_rating(char *given_vin) {
  int vin_counter = 0;
  int same_vin_count = 0;
  int vin_index = -1;
  float average = 0.0;
  for (int i = 0; i < VIN_LEN; i++) {
    if (given_vin[i] != '\0') {
      if ((isupper(given_vin[i])) || (isdigit(given_vin[i]))) {
        if ((given_vin[i] != 73) && (given_vin[i] != 79) &&
        (given_vin[i] != 81)) {
          vin_counter++;
        }
      }
    }
  }

  if (vin_counter != 17) {
    return INVALID_VIN;
  }
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 17; j++) {
      if (given_vin[j] == g_vin[i][j]) {
        same_vin_count++;
      }
    }
    if (same_vin_count == 17) {
      vin_index = i;
    }
    same_vin_count = 0;
  }

  if (vin_index == -1) {
    return NO_SUCH_DATA;
  }


  average = (g_rating[vin_index][0] + g_rating[vin_index][1] +
  g_rating[vin_index][2]) / 3;


  return average;
} /* calculate_avg_rating() */

/* This function returns a file that contains that contains the
 * the vin and the associated best rating out of the three, comfort,
 * performance, reliability. The first parameter, out_file, is the file
 * which the information is written to.
 */

int show_best_rating_for_each_car(char* out_file) {
  FILE *output = fopen(out_file, "w");
  char rating[50];
  int rating_ind = 0;
  float comfort = 0.0;
  float performance = 0.0;
  float reliability = 0.0;

  if (output != NULL) {
    if (g_cars_count == 0) {
      fclose(output);
      output = NULL;
      return NO_DATA_POINTS;
    }

    for (int i = 0; i < 50; i++) {
      performance = g_rating[i][0];
      comfort = g_rating[i][1];
      reliability = g_rating[i][2];
      if ((comfort > reliability) && (comfort > performance)) {
        rating[rating_ind] = 'c';
      }
      else if ((reliability > comfort) && (reliability > performance)) {
        rating[rating_ind] = 'r';
      }
      else if ((performance > comfort) && (performance > reliability)) {
        rating[rating_ind] = 'p';
      }
      else {
        rating[rating_ind] = 'm';
      }
      rating_ind++;
    }

    for (int i = 0; i < 50; i++) {
      if (g_price[i] != '\0') {
        if (rating[i] == 'c') {
          fprintf(output, "%s: Comfort\n", g_vin[i]);
        }
        else if (rating[i] == 'r') {
          fprintf(output, "%s: Reliability\n", g_vin[i]);
        }
        else if (rating[i] == 'p') {
          fprintf(output, "%s: Performance\n", g_vin[i]);
        }
        else {
          fprintf(output, "%s: More than one\n", g_vin[i]);
        }
      }
    }

    for (int i = 0; i < 50; i++) {
      for (int j = 0; j < 17; j++) {
        g_vin[i][j] = '\0';
      }
      g_price[i] = '\0';
    }
    fclose(output);
    output = NULL;
    return OK;
  }
  return FILE_WRITE_ERROR;
} /* show_best_rating_for_each_car() */

/* This function writes certain information into a file given
 * a range of columns. The first parameter, out_file, is the file
 * the information will be written to. The third parameter, start_col,
 * is where the indexing starts while reading information from the table
 * with columns. The fourth parameter, end_col, is where the indexing ends.
 */

int write_tables(char *out_file, int start_col, int end_col) {
  FILE *output = fopen(out_file, "w");
  if (output != NULL) {
    if (g_cars_count == 0) {
      fclose(output);
      output = NULL;
      return NO_DATA_POINTS;
    }

    if ((start_col < 0) || (end_col < 0)) {
      fclose(output);
      output = NULL;
      return INVALID_COLUMN;
    }
    else if (end_col < start_col) {
      fclose(output);
      output = NULL;
      return INVALID_COLUMN;
    }

    for (int i = 0; i < 50; i++) {
      if (g_price[i] != '\0') {
        for (int j = start_col; j < end_col; j++) {
          fprintf(output, "%s,", g_basic_info[i][j]);
        }
        fprintf(output, "%s\n", g_basic_info[i][end_col]);
      }
    }

    for (int i = 0; i < 50; i++) {
      for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 50; k++) {
          g_basic_info[i][j][k] = '\0';
        }
      }
      g_price[i] = '\0';
    }
    fclose(output);
    output = NULL;
    return OK;
  }
  return FILE_WRITE_ERROR;
} /* write_tables() */
