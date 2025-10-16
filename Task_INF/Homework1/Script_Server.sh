


while read -p "Сообщение: " message; do
    if [ "$message" = "exit" ]; then
        echo "Выход"
        exit 0
    fi
    echo "$message" | ncat localhost 12345
    echo "Отправлено"
done
