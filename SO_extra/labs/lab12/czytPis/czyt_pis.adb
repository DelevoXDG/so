with Ada.Text_IO; 
use Ada.Text_IO;

procedure Czyt_Pis is

task type Czytelnia(N: Integer) is
  entry chcePisac;
  entry chceCzytac;
  entry koniecPisania;
  entry koniecCzytania;
end Czytelnia;

task body Czytelnia is
  i: Integer;
begin
  i:= 0;
end Czytelnia;

C: Czytelnia(3);

task type Pisarz(i,p: Integer) is
end Pisarz;

task body Pisarz is
  id: Integer;
begin
  id:= i;
  delay 0.1;
  for l in 1..p loop
    C.chcePisac;
    Put_Line("P" & Integer'Image(id) & " <- IN");
    delay 0.5;
    Put_Line("P" & Integer'Image(id) & " -> OUT");
    C.koniecPisania;
  end loop;
end Pisarz;

task type Czytelnik(i,p: Integer) is
end Czytelnik;

task body Czytelnik is
  id: Integer;
begin
  id:= i;
  delay 0.1;
  for l in 1..p loop
    C.chceCzytac;
    Put_Line("C" & Integer'Image(id) & " <- IN");
    delay 0.4;
    Put_Line("C" & Integer'Image(id) & " -> OUT");
    C.koniecCzytania;
  end loop;
end Czytelnik;

C1: Czytelnik(1,5);
C2: Czytelnik(2,5);
C3: Czytelnik(3,5);
C4: Czytelnik(4,5);
P1: Pisarz(1,10);
P2: Pisarz(2,5);

begin
  Put_Line("OK");
end Czyt_Pis;