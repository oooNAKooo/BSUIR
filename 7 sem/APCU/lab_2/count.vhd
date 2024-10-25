library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity priority_encoder is 
    Port (
        inputs : in std_logic_vector(8 downto 0);
        outputs : out std_logic_vector(3 downto 0)
    );
end priority_encoder;

architecture Behavioral of priority_encoder is 
begin
    process(inputs)
    begin
        -- Инициализация выходов
        outputs <= "0000";

        -- Приоритетный кодировщик
        if inputs(8) = '1' then
            outputs <= "1001"; -- 9
        elsif inputs(7) = '1' then
            outputs <= "1000"; -- 8
        elsif inputs(6) = '1' then
            outputs <= "0111"; -- 7
        elsif inputs(5) = '1' then
            outputs <= "0110"; -- 6
        elsif inputs(4) = '1' then
            outputs <= "0101"; -- 5
        elsif inputs(3) = '1' then
            outputs <= "0100"; -- 4
        elsif inputs(2) = '1' then
            outputs <= "0011"; -- 3
        elsif inputs(1) = '1' then
            outputs <= "0010"; -- 2
        elsif inputs(0) = '1' then
            outputs <= "0001"; -- 1
        else
            outputs <= "0000"; -- 0
        end if;
    end process;
end Behavioral;
