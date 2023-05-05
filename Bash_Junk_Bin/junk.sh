#!/bin/bash

#################################
# Homework 1 - Team 13
# Name: Mario Carrillo
# uni: mc5132
# version: 1.0 January 31, 2022
#################################

num_flag=0 #count the number of distinct flags, max 3
help_display_flag=0 #initialize the flags to be like boolean values. In this case, they are set to false
list_junk_files_flag=0
purge_all_files_flag=0
num_h_flag=0 #specific count of the flags
num_l_flag=0 
num_p_flag=0

readonly JUNK=~/.junk #readonly junk directory

function usage_here_doc() { #create a function for the here doc as we are going to repeatedly use the here doc
        cat << ENDOFTEXT
Usage: $(basename "$0") [-hlp] [list of files]
    -h: Display help.
    -l: List junked files.
    -p: Purge all files.
    [list of files] with no other arguments to junk those files.
ENDOFTEXT
}

while getopts ":hlp" option; do #while loop to iterate through the command line arguments and looks for valid command line arguments
	case "$option" in
		h) help_display_flag=1 #if we see the help flag, set the value to true
			if [ $num_h_flag -eq 0 ]; then #if this is the first h flag we see 
				num_flag=$(( num_flag+1 )) #increment the count of distinct flags
			fi
			num_h_flag=$(( num_h_flag+1 )) #increase the amt of h flags
			;;
		l) list_junk_files_flag=1 #if we see the junk flag, set the value to true
			if [ $num_l_flag -eq 0 ]; then #smae like the previous one, but with l
                                num_flag=$(( num_flag+1 )) #increment the count of distinct flags
                        fi
			num_l_flag=$(( num_l_flag+1 ))
			;;
		p) purge_all_files_flag=1 #if we see the purge flag, set the value to true
			if [ $num_p_flag -eq 0 ]; then
                                num_flag=$(( num_flag+1 ))
                        fi
			num_p_flag=$(( num_p_flag+1 ))
			;;
		?) printf "Error: Unknown option '-%s'.\n" "$OPTARG" >&2 #>&2 is standard error in java
			usage_here_doc
          		exit 1
          		;;
	esac
done

shift "$((OPTIND-1))" #shift the command line arguments to no longer read the flags 

if [ $(( num_flag )) -eq 0 ] && [ $# -eq 0 ]; then #if we saw 0 distinct flags, and there are no command line argumnets
	usage_here_doc #show the here doc
elif [ $(( num_flag )) -gt 1 ]; then #else if we see more than one distinct flag, we have too many arguments
        echo "Error: Too many options enabled." >&2
	usage_here_doc
	exit 1 #exit with failure
elif [ $(( num_flag )) -eq 1 ] && [ ! -z "$1" ]; then #else if we have one distinct flag and we have other arguments that are non flags, we have too many arguments
        echo "Error: Too many options enabled." >&2
        usage_here_doc
	exit 1 #exit with failure
elif [ $(( help_display_flag )) -eq 1 ] && [ $(( list_junk_files_flag )) -eq 0 ] && [ $(( purge_all_files_flag )) -eq 0 ]; then #else if we have just the -h flag, then show the help 
	usage_here_doc
elif [ $(( list_junk_files_flag )) -eq 1 ] && [ $(( help_display_flag )) -eq 0 ] && [ $(( purge_all_files_flag )) -eq 0 ]; then #else if we have just the -l flag, do ls -lAF in the junk directory
	ls -lAF $JUNK
elif [ $(( purge_all_files_flag )) -eq 1 ] && [ $(( help_display_flag )) -eq 0 ] && [ $(( list_junk_files_flag )) -eq 0 ]; then #else if we have just the -p flag, iterate through the files/directories in the junk directory and remove them
	for file in $JUNK; do
		if [ -f "$file" ]; then
			rm "$file"
		elif [ -d "$file" ]; then
			rm -r "$file"
		fi
	done
fi

if [ ! -d "$JUNK" ]; then #if the junk directory does not exist, make the junk directory
	mkdir $JUNK
fi

if [ $num_flag -eq 0 ] && [ $# -gt 0 ]; then #if the number of distinct flags we saw is 0, but we have command line arguments	
	for file in "$@"; do #for the arguments given, check if they are files or directories and move them to the junk directory
		if [ -f "$file" ]; then
			mv "$file" $JUNK
		elif [ -d "$file" ]; then
			mv "$file" $JUNK
		else
			echo "Warning: '"$file"' not found" >&2 #else return an error
		fi
	done
	exit 0
fi

exit 0 #exit with success
