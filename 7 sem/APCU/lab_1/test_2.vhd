library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Multiplexer16x1_tb is
end Multiplexer16x1_tb;

architecture Behavioral of Multiplexer16x1_tb is

    signal A : STD_LOGIC_VECTOR(3 downto 0);    -- Входы выбора
    signal D : STD_LOGIC_VECTOR(15 downto 0);   -- Входы данных
    signal Y : STD_LOGIC;                       -- Выход

    component Multiplexer16x1
        Port ( A : in STD_LOGIC_VECTOR(3 downto 0);
               D : out STD_LOGIC_VECTOR(15 downto 0);
               Y : out STD_LOGIC);
    end component;

begin

    UUT: Multiplexer16x1 port map (A => A, D => D, Y => Y);

    process
    begin
        A <= "0000";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;                             -- Ожидание 5 наносекунд
        
        A <= "0001";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;  

        A <= "0010";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;                             -- Ожидание 5 наносекунд

        A <= "0011";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;  

        A <= "0100";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;  

        A <= "0101";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;  

        A <= "0110";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;  

        A <= "0111";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;  

        A <= "1000";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;  
  
        A <= "1001";  -- Вход D0 = 1, остальные нули
        wait for 10 ns; 

        A <= "1111";  -- Вход D0 = 1, остальные нули
        wait for 10 ns;               

        wait;
    end process;

end Behavioral;
