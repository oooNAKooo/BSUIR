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
        outputs <= "0000"; -- Значение по умолчанию

        -- Приоритетный кодировщик с использованием оператора case
        case inputs is
            when "100000000" => -- 9
                outputs <= "1001";
            when "010000000" => -- 8
                outputs <= "1000";
            when "001000000" => -- 7
                outputs <= "0111";
            when "000100000" => -- 6
                outputs <= "0110";
            when "000010000" => -- 5
                outputs <= "0101";
            when "000001000" => -- 4
                outputs <= "0100";
            when "000000100" => -- 3
                outputs <= "0011";
            when "000000010" => -- 2
                outputs <= "0010";
            when "000000001" => -- 1
                outputs <= "0001";
            when others => -- Все нули
                outputs <= "0000";
        end case;
    end process;
end Behavioral;
