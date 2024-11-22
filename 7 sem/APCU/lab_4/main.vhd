library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity artix is
    Port(
        ledsmain: out std_logic_vector(3 downto 0);     --светодиоды
        ledsboard: out std_logic_vector(3 downto 0);    --светодиоды
        pushbuttons:in std_logic_vector(4 downto 0);    --кнопки
        dipswitch:in std_logic_vector(3 downto 0);      --переключатели
        sysclk_p:in std_logic;                          --принимают дифф тактовый сигнал
        sysclk_n:in std_logic
    );
end artix;

architecture Behavioral of artix is

     component UpDownCounter
            port ( 
                 RST : in STD_LOGIC;
                 CLK : in STD_LOGIC;
                 UP  : in  STD_LOGIC; 
                 DOWN: in  STD_LOGIC;
                 LOAD: in  STD_LOGIC;
                 PRESET   : in  STD_LOGIC_VECTOR(3 downto 0);
                 Q: out STD_LOGIC_VECTOR(3 downto 0)
            );
      end component;
  
    component ibufds            
    port (
        I, IB : in std_logic;   --
        O     : out std_logic); --
    end Component;
        
    component clk_div is    --делитель частоты
    port (
        clk_in  : in std_logic;
        clk_out : out std_logic);
    end Component;
    
    signal PRESET : std_logic_vector(3 downto 0);
    signal Q: std_logic_vector(3 downto 0);
    signal clk_no_div: std_logic;
    signal clk_divided: std_logic;
begin

    counter_uut: UpDownCounter
    Port Map (
      RST => pushbuttons(1), --левая
      CLK => clk_divided,
      LOAD => pushbuttons(0), --верхняя
      PRESET => PRESET,
      UP => pushbuttons(2), --центральная
      DOWN=> pushbuttons(3), --правая
      Q => Q
    );
    
    PRESET(3 downto 0) <= dipswitch(3 downto 0);
    ledsmain(3 downto 0) <= Q(3 downto 0);
    
    buff: ibufds
    Port Map (
        I => sysclk_p, 
        IB=> sysclk_n,
        O => clk_no_div
    );
    
    divider_uut: clk_div
    Port Map (
        clk_in => clk_no_div,
        clk_out => clk_divided
    );

end Behavioral;
