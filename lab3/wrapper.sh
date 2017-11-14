bin=${source%.*}

if ! make; then
    echo "ERROR: Failed to compile file."
    exit 1
fi

if ! ls tests; then
    mkdir -p tests
fi
if ! python3 generate.py; then
    echo "ERROR: Failed to python generate tests."
    exit 1
fi
path = './tests/'
for test_file in `ls tests/*.t`; do
    answer_file="${test_file%.*}"
    echo "Execute ${test_file}"
    if !  ./main 10 < $test_file > "${answer_file}.pl" ; then
        echo "ERROR"
        continue
    fi

    if ! diff -u -b "${answer_file}.txt"  "${answer_file}.pl"; then
        echo "Failed"
    else
        echo "OK"
    fi
done  
if ! rm -rf tests; then
    echo "ERROR: Folder no rm."
    exit 1
fi

