library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use std.textio.all;
use IEEE.std_logic_textio.all;

entity testbench is 
end testbench;

architecture behavior of testbench is

    signal input_lines : std_logic_vector(8 downto 0);
    signal output_lines : std_logic_vector(3 downto 0);
    signal expected_output : std_logic_vector(3 downto 0);

    component bcd_priority_encoder 
        port (
            input_lines : in std_logic_vector(8 downto 0);
            output_lines : out std_logic_vector(3 downto 0)
        );
    end component;

    -- Объявление файлового типа
    file file_input : text; -- Правильное объявление файла

begin

    uut: bcd_priority_encoder 
        port map (
            input_lines => input_lines,
            output_lines => output_lines
        );

    process
        variable line_input : line;
        variable temp_input : std_logic_vector(8 downto 0);
        variable temp_expected : std_logic_vector(3 downto 0);
        variable temp_output : std_logic_vector(3 downto 0);
    begin
        -- Открытие файла для чтения
        file_open(file_input, "input_data.txt", read_mode);

        while not endfile(file_input) loop
            readline(file_input, line_input);
            read(line_input, temp_input);
            read(line_input, temp_expected);
            read(line_input, temp_output);

            input_lines <= temp_input;
            expected_output <= temp_expected;
            output_lines <= temp_output;

            wait for 10 ns; -- Задержка для симуляции

            assert output_lines = expected_output
                report "Ошибка: выходные данные не совпадают с ожидаемыми"
                severity error;
        end loop;

        file_close(file_input);
        wait;
    end process;

end behavior;
