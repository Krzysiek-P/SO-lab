# Moduł ImportExcel
Import-Module -Name ImportExcel

# Ścieżka do plików Excel
$adresyPlik = "C:\users\Home\adresy.xlsx"
$wynikiPlik = "C:\users\Home\wyniki.xlsx"

# Otwieramy plik Excel i odczytuj dane z arkusza "IP-Addresses"
$adresy = Import-Excel -Path $adresyPlik -WorksheetName "IP-Addresses"

# Sprawdzamy, czy adresy zostały poprawnie odczytane
if ($adresy -eq $null -or $adresy.Count -eq 0) {
    Write-Host "Nie znaleziono żadnych adresów IP w pliku Excel."
    exit
}

# Tablicę na wyniki
$wyniki = @()

# Wykonujemy test połączenia dla pierwszych 5 adresów
$adresy | Select-Object -First 5 | ForEach-Object {
    $ipAdres = $_.PSObject.Properties.Name # Odczytaj adres IP z klucza
    $wynik = Test-Connection -ComputerName $ipAdres -Count 1 -Quiet

    # Dodaj wynik do tablicy wyników
    $wyniki += [PSCustomObject]@{
        Adres = $ipAdres
        Wynik = if ($wynik) { "Połączenie udane" } else { "Brak połączenia" }
    }
}

# Zapisz wyniki do pliku Excel
$wyniki | Export-Excel -Path $wynikiPlik -WorksheetName "Wyniki" -AutoSize

Write-Host "Skrypt zakończony sukcesem. Wyniki zapisano w: $wynikiPlik"
