filename="$1"
text="$2"


echo "Файл: $filename"
echo "Текст: $text"


if [ -f "$filename" ]; then
    echo "Файл $filename существует"
else
    echo "Файл $filename не существует - создаем новый"
    touch "$filename"
    echo "Файл создан!"
fi

echo "Записываем текст в файл"
echo "$text" >> "$filename"
echo "Текст записан в файл"

while read line; do
	echo ">>> $line"
done < "$filename"
echo "Файл прочитан"

ncat -l -p 12345 >> "$filename" 
