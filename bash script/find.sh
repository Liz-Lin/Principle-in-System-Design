#! /bin/bash
set -f
file=$1
shift;
expression=$@


evaluate_expression()
{

    if [ $# -eq 0 ]; then
        echo no more argus
        return;
    else
    file_m=$1
    shift;
    expression_m=($@)


case ${expression_m[0]} in
    (-print)
    echo $file_m
    evaluate_expression $file_m ${expression_m[@]:1}
    ;;
    (-name)
    if [[ ${expression_m[1]} == *"*"* ]];
    then
    new_name=`echo ${expression_m[1]} | tr -d * `
        if [[ $file_m = *"$new_name" ]];
        then
        evaluate_expression $file_m ${expression_m[@]:2};
        fi
    else
        if [[ $file_m =~ "${expression_m[1]}" ]];
        then
        evaluate_expression $file_m ${expression_m[@]:2};
        fi
    fi
    ;;
    (-type)
    case ${expression_m[1]} in
            (d)
            if [ -d $file_m ];
            then
            evaluate_expression $file_m ${expression_m[@]:2};
            fi
            ;;
            (f)
            if [ -f $file_m ];
            then
            evaluate_expression $file_m ${expression_m[@]:2};
            fi
            ;;
            (*)
            echo wrong
            ;;
            esac
    ;;

    (-exec)
    to_exec=`echo  ${expression_m[@]:1} | sed -e s#{}#$file_m#g# `
    $to_exec
    ;;
    (*)
    ;;
    esac
fi
}


my_find()
{

    evaluate_expression $file $expression
    if [ -d $file ];
    then
        files_in_dir=(`ls -A $1`)
        for f in ${files_in_dir[*]}
        do
            evaluate_expression $1/$f $expression;
            if [ -d $1/$f ];
            then
            my_find $1/$f $expression;
            fi
        done;
    fi
}

my_find $file $expression





