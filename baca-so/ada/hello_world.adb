with Ada.Text_IO; use Ada.Text_IO;
with Rozwiazanie;

procedure Hello_World is
   use Rozwiazanie;

   iloscPracownikow : Integer := 7;

   serwer :
     SerwerSzkolen
       (iloscPracownikow, 5); -- druga liczba tutaj określa rozmiar grupy

   task type Pracownik is
      entry NadajId (x : in Integer);
   end Pracownik;

   task body Pracownik is
      id      : Integer := 1;
      rozmiar : Integer;
      inni    : ListaUczestnikow;

   begin
      accept NadajId (x : in Integer) do
         id := x;
      end NadajId;

      Put_Line ("przed zapisem");
      serwer.Zapisz (id);
      Put_Line ("po zapisie");

      serwer.Dolacz (id, rozmiar, inni);
      Put_Line ("po dolaczeniu");

      for i in 1 .. rozmiar loop
         Put_Line (Integer'Image (inni (i)));
      end loop;

      serwer.Opusc (id);
      Put_Line ("po opuszczeniu");
   end Pracownik;

   Pracownicy : array (1 .. iloscPracownikow) of Pracownik;

begin
   for i in 1 .. iloscPracownikow loop
      Pracownicy (i).NadajId (i);
   end loop;
end Hello_World;
