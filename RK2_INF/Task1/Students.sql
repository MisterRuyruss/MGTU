-- Создание таблиц
CREATE TABLE students (
    student_id SERIAL PRIMARY KEY,
    full_name VARCHAR(100) NOT NULL,
    group_number VARCHAR(20) NOT NULL
);

CREATE TABLE subjects (
    subject_id SERIAL PRIMARY KEY,
    subject_name VARCHAR(100) NOT NULL
);

CREATE TABLE grades (
    grade_id SERIAL PRIMARY KEY,
    student_id INT NOT NULL REFERENCES students(student_id),
    subject_id INT NOT NULL REFERENCES subjects(subject_id),
    grade INT CHECK (grade BETWEEN 1 AND 5)
);

CREATE TABLE attendance (
    attendance_id SERIAL PRIMARY KEY,
    student_id INT NOT NULL REFERENCES students(student_id),
    date_attended DATE NOT NULL,
    status VARCHAR(20) CHECK (status IN ('Присутствовал', 'Отсутствовал'))
);

CREATE TABLE notes (
    note_id SERIAL PRIMARY KEY,
    student_id INT NOT NULL REFERENCES students(student_id),
    note_text TEXT NOT NULL
);

-- Наполнение данными
INSERT INTO students (full_name, group_number) VALUES
('Иванов Иван Иванович', 'Группа 101'),
('Петров Петр Петрович', 'Группа 101'),
('Сидорова Анна Сергеевна', 'Группа 101'),
('Кузнецов Алексей Викторович', 'Группа 101'),
('Смирнова Ольга Дмитриевна', 'Группа 101'),
('Васильев Денис Андреевич', 'Группа 101');

INSERT INTO subjects (subject_name) VALUES
('Математический анализ'),
('Аналитическая геометрия'),
('Информатика');

INSERT INTO grades (student_id, subject_id, grade)
SELECT s.student_id, sub.subject_id, 
       (CASE 
            WHEN sub.subject_name = 'Математический анализ' THEN 4
            WHEN sub.subject_name = 'Аналитическая геометрия' THEN 5
            WHEN sub.subject_name = 'Информатика' THEN 3
        END) as grade
FROM students s
CROSS JOIN subjects sub;

INSERT INTO attendance (student_id, date_attended, status)
SELECT student_id, '2025-03-01'::DATE, 'Присутствовал'
FROM students
UNION ALL
SELECT student_id, '2025-03-02'::DATE, 'Отсутствовал'
FROM students
WHERE student_id IN (2, 4);

INSERT INTO notes (student_id, note_text) VALUES
(7, 'Любит информатику'),
(8, 'Нужна помощь по информатике'),
(9, 'Редко посещает занятия по информатике'),
(10, 'Отличник по всем предметам'),
(11, 'Хорошо работает в команде'),
(12, 'Прогресс по информатике');

-- Создание индексов
CREATE INDEX idx_students_group_number ON students(group_number);
CREATE INDEX idx_grades_student_id ON grades(student_id);
CREATE INDEX idx_notes_note_text_gin ON notes USING gin(to_tsvector('russian', note_text));

-- Создание представления
CREATE VIEW student_avg_grades AS
SELECT 
    s.student_id,
    s.full_name,
    ROUND(AVG(g.grade)::numeric, 2) AS average_grade
FROM students s
JOIN grades g ON s.student_id = g.student_id
GROUP BY s.student_id, s.full_name;

SELECT * FROM student_avg_grades;

-- Создание тракзаций
BEGIN; 
WITH new_student AS (
    INSERT INTO students (full_name, group_number) 
    VALUES ('Медведев Андрей Эдуардович', 'Группа 101')
    RETURNING student_id
)
INSERT INTO grades (student_id, subject_id, grade)
SELECT new_student.student_id, subject_id, grade_value
FROM new_student
CROSS JOIN (VALUES 
    (4, 5),
    (5, 5),
    (6, 5)
) AS subjects_grades(subject_id, grade_value);
COMMIT; 

SELECT * FROM students ORDER BY student_id DESC LIMIT 1;
SELECT * FROM grades WHERE student_id = (SELECT student_id FROM students WHERE full_name LIKE 'Новиков%');

--Запросы
WITH numbered_students AS (
    SELECT 
        student_id,
        full_name,
        group_number,
        ROW_NUMBER() OVER (ORDER BY student_id) as rn
    FROM students
    WHERE group_number = (SELECT group_number FROM students WHERE student_id = 10)
)
SELECT student_id, full_name, group_number
FROM numbered_students
WHERE rn BETWEEN 
    (SELECT rn FROM numbered_students WHERE student_id = 10) - 2 AND
    (SELECT rn FROM numbered_students WHERE student_id = 10) + 3
  AND student_id != 10
ORDER BY rn;


SELECT * FROM student_avg_grades 
WHERE student_id = 15;


SELECT 
    s.subject_name,
    ROUND(AVG(g.grade)::numeric, 2) AS avg_grade
FROM grades g
JOIN subjects s ON g.subject_id = s.subject_id
WHERE s.subject_name = 'Информатика'
GROUP BY s.subject_name;


SELECT note_id, student_id, note_text
FROM notes
WHERE to_tsvector('russian', note_text) @@ to_tsquery('russian', 'Информатика');



BEGIN;

UPDATE attendance 
SET status = 'Отсутствовал' -- или 'Присутствовал'
WHERE student_id = 10 
  AND date_attended = '2025-03-01'::DATE;

COMMIT;