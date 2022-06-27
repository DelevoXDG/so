--Maksim Zdobnikau
-- with Ada.Text_IO; use Ada.Text_IO;

package body Rozwiazanie is

   task body SerwerSzkolen is
-- tutaj pomocnicze zmienne/procedury
      ongoingTraining : Boolean := False;
      waitNum         : Integer := 0;
      waitArr         : ListaUczestnikow;
      doneNum         : Integer := 0;
      didntLeaveNum   : Integer := 0;
   begin
-- tutaj kod rozwiazania
      while (doneNum < liczbaPracownikow) loop
         select when (not ongoingTraining and waitNum <= maxWSzkoleniu) =>
            accept Zapisz (id : in Integer) do
               -- Put_Line ("Zapisz");
               waitNum           := waitNum + 1;
               didntLeaveNum     := didntLeaveNum + 1;
               waitArr (waitNum) := id;
               if
                 (waitNum >= rozmiarGrupy or
                  waitNum >= liczbaPracownikow - doneNum)
               then
                  ongoingTraining := True;
               end if;
            end Zapisz;
         or when (ongoingTraining = True) =>
            accept Dolacz
              (id         : in     Integer; --
               rozmiar    :    out Integer; --
               uczestnicy :    out ListaUczestnikow)
            do
               -- Put_Line ("Dolacz");
               rozmiar    := waitNum;
               uczestnicy := waitArr;
            end Dolacz;
         or when (didntLeaveNum > 0) =>
            accept Opusc (id : in Integer) do
               -- Put_Line ("Opusc");
               doneNum := doneNum + 1;
               if (didntLeaveNum > 1) then
                  didntLeaveNum := didntLeaveNum - 1;
               else
                  waitNum         := 0;
                  didntLeaveNum   := 0;
                  ongoingTraining := False;
               end if;
            end Opusc;
         end select;
      end loop;
   end SerwerSzkolen;

end Rozwiazanie;
