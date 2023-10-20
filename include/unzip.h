

void unzip(char *input_filepath, char *output_filepath);
void ungzip(char *input_filepath, char *output_filepath);

char * is_df_zipped(char *ptr_in_ff, char *filetype);

char * get_unzipped_filepath(char *filepath, char *zip_ext);