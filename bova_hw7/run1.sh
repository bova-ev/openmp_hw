progname="bugged1"

threads=-1
params=""

while [ $# -gt 0 ]
do
    case "$1" in
        -t)
        shift
        if [ $# -gt 0 ] && [ ${1} != "-p" ]; then
            threads=$1
        else
            echo "Invalid number of threads"
            exit 1
        fi
        shift
        ;;
        -p)
        shift
        while [ $# -gt 0 ] && [ ${1} != "-t" ]; do
            params="${params} $1"
            shift
        done
        ;;
        -h|--help)
        echo "-h | --help  Show this message."
        echo "-t <number>  Set <number> of threads."
        echo "-p <params>  Pass <params> to the program."
        exit 0
        ;;
        *)
        echo "Invalid arguments for run.sh. Try running \"./run.sh -h\""
        exit 1
        ;;
    esac
done

echo "gcc ${progname}.c -o ${progname} -fopenmp"
gcc ${progname}.c -o ${progname} -fopenmp

if [ $threads -gt 0 ]; then
    echo "OMP_NUM_THREADS=$threads ./${progname} ${params}"
    OMP_NUM_THREADS=$threads ./${progname} ${params}
else
    echo "./${progname} ${params}"
    ./${progname} ${params}
fi

rm ${progname}

