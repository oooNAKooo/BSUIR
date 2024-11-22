library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity UpDownCounter is
    Port ( CLK     : in  STD_LOGIC;
           RST     : in  STD_LOGIC;  -- Асинхронный сброс
           UP      : in  STD_LOGIC;  -- Счет вверх
           DOWN    : in  STD_LOGIC;  -- Счет вниз
           PRESET   : in  STD_LOGIC_VECTOR(3 downto 0); -- Предустановка
           LOAD     : in  STD_LOGIC;  -- Загрузка предустановленного значения
           Q        : out STD_LOGIC_VECTOR(3 downto 0)); -- Выход счетчика
end UpDownCounter;

architecture Behavioral of UpDownCounter is
    signal count : STD_LOGIC_VECTOR(3 downto 0) := (others => '0');
begin

    process(CLK, RST)
    begin
        if (RST = '1') then
            count <= (others => '0');  -- Сброс счетчика
        elsif rising_edge(CLK) then
            if (LOAD = '1') then
                count <= PRESET;  -- Загрузка предустановленного значения
            elsif (UP = '1' and DOWN = '0') then
                count <= count + 1;  -- Счет вверх
            elsif (DOWN = '1' and UP = '0') then
                count <= count - 1;  -- Счет вниз
            end if;
        end if;
    end process;

    Q <= count;  -- Выход счетчика

end Behavioral;
