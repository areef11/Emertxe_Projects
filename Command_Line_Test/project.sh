#!/bin/bash

<<doc
Name		: Areef Ahmad
Date		: 20-11-2022
Description	: LS - Project: Command Line Test
doc

starttest(){
	echo -e "\n\n\e[32mWELCOME TO COMMAND LINE TEST\e[39m\n\e[33mEnter the correct option for each question.\nYou have 10 seconds to answer each question.\e[39m\n\n"
	lines=`wc -l questionbank.txt | cut -d " " -f1`

	for i in `seq 5 5 $lines`
	do
		head -$i questionbank.txt | tail -5
		for j in `seq 10 -1 1`
		do
			echo -n -e "\rEnter the option: $j "
			read -t 1 option
			
			if [ -n "$option" ]
			then
				break
			else
				option="e"
				`echo "$option" >> username.txt`
			fi
		done	
		`echo "$option" >> useranswer.txt`
	done

	userarray=(`cat useranswer.txt`)
	answerarray=(`cat correctanswer.txt`)
	answerlength=$((${#answerarray[@]}-1))
	count=0
	headvalue=5

	echo -e "\n\e[96m YOUR RESULT IS AS FOLLOWS\e[39m\n"

	for i in `seq 0 $answerlength`
	do
		head -$headvalue questionbank.txt | tail -5
		echo "Your Answer: "${userarray[$i]}" "
		echo "Correct Answer: "${answerarray[$i]}" "
		if [ ${userarray[$i]} = ${answerarray[$i]} ]
		then
			echo -e "\e[32mCorrect Answer\e[39m"
			echo "Your Answer: "${userarray[$i]}" "
			echo "Correct Answer: "${answerarray[$i]}" "
			count=$(($count+1))
		elif [ ${userarray[$i]} != ${answerarray[$i]} ]
		then
			echo -e "\e[91mWrong Answer\e[39m"
			echo "Your Answer: "${userarray[$i]}" "
			echo "Correct Answer: "${answerarray[$i]}" "
		fi

		headvalue=$(($headvalue+5))
	done

	echo -e "Your score is $count/10.\n"

	echo -e "\e[32mTHANK YOU\e[39m"
}

aftersignin()
{
	echo -e "\n1.Take test now\n2.Exit\n"
	read -p "Enter your choice: " choice 
	case $choice in
		1) starttest
			;;
		2) exit
			;;
	esac
}
password()
{
	echo -e "\nEnter Password: "
	read -s password1
	echo "Confirm Password: "
	read -s password2

	if [ $password1 = $password2 ]
	then
		`echo $username >> user.csv`
		`echo $password1 >> password.csv`
		echo "Account created successfully"
		aftersignin
	else
		echo "Password confirmation failed, please try again."
		password
	fi
}

signup()
{
	read -p "Enter Username: " username
	arr=(`cat user.csv`)
	count=0

	for i in ${arr[@]}
	do
		if [ $username = $i ]
		then
			count=$(($count+1))
		fi
	done
	if [ $count -eq 0 ]
	then
		password
	else
		echo "The username $username, already exists, please try again."
		signup
	fi
}

signin()
{
	read -p "Enter username: " username

	usernames=(`cat user.csv`)
	passwords=(`cat password.csv`)
	length=${#usernames[@]}
	count=0

	for i in `seq 0 $(($length-1))`
	do
		if [ ${usernames[$i]} = $username ]
		then
			count=$(($count+1))
			index=$i
		fi
	done

	if [ $count -eq 0 ]
	then
		echo "Username does not exist,  please sign up to register"
		main
	else
		echo -e "\n"
		read -s -p "Enter password: " pass
		
		if [ $pass = ${passwords[$index]} ]
		then
			echo "You have successfully signed in."
			aftersignin
		else
			echo "Incorrect passowrd, please try again."
			signin
		fi
		
	fi
}

main(){
echo -e "\e[1m1. Sign Up\n2. Sign In\n3. Exit\n\e[21m"

read -p "Enter your choice: " choice
echo -e "\n"
case $choice in
	1) signup
		;;
	2) signin
		;;
esac
}

main
