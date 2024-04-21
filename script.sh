#!/bin/bash

script(){
	local path="$1"
	local flags="$2"
	
	#Parcours des répoertoires et sous-répertoires
	find "$path" -type f -name '*.c' | sort -V| while read file_path; do
		echo "##### $(basename "$(dirname "$file_path")") #####"
		echo "Executing: $file_path with flags $flags"
		command="./minicc $flags $file_path"
		result=$(eval "$command" 2>&1)
		if [ -z "$result" ]; then
			echo -e "\t $(dirname "$file_path")/$(basename "file_path"): OK\n"
		else
			echo -e "\t $(dirname "$file_path")/$(basename "file_path"): $result\n"
		fi
	done
}

script "Tests/Syntaxe" "-s"
script "Tests/Verif" "-v"

#Ne pas oublier chmod +x script.sh dans le terminal avant la premiere utilisation
