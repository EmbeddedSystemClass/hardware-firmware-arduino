Dim line As Shape
Dim sa(0 To 60) As Integer

Const ACCURACY = 10000

Sub drawClock(px As Integer, py As Integer, size As Integer)
    Dim ws As Worksheet
    
    Set ws = ActiveSheet
    
    While (ws.Shapes.Count > 0)
        ws.Shapes(0).Delete
    Wend
        
    Dim i As Integer
    Dim s As Integer
    Dim x As Integer
    Dim y As Integer
    Dim pi As Double
        
    
        
    pi = 3.14159265358979
    
    For i = 0 To 60
        sa(i) = Sin(i * 6 * pi / 180) * ACCURACY
    Next
    
    drawScale px, py, size
    
    Do While (True)
        s = Second(Now)
        's = 15
        'x = 50 * Cos(s * 6 * pi / 180)
        'y = 50 * Sin(s * 6 * pi / 180)
        ws.Cells(1, 1) = s
        s = (s + 45) Mod 60
        x = sa((s + 15) Mod 60) / ACCURACY * size
        y = sa((s) Mod 60) / ACCURACY * size
        
        drawLine px, py, x + px, y + py
        
        
        
        ws.Cells(2, 1) = x
        ws.Cells(3, 1) = y
        
        DoEvents
   Loop
   
End Sub

Sub drawLine(x1 As Integer, y1 As Integer, x2 As Integer, y2 As Integer)
    Dim ws As Worksheet
    
    Set ws = ActiveSheet
    On Error Resume Next
    ws.Shapes(ws.Shapes.Count).Delete
    
    ws.Shapes.AddLine x1, y1, x2, y2
   
End Sub

Sub drawScale(px As Integer, py As Integer, size As Integer)
    Dim i As Integer
    Dim x1 As Integer
    Dim y1 As Integer
    Dim x2 As Integer
    Dim y2 As Integer
    
    For i = 0 To 59 Step 5
        x1 = sa((i + 15) Mod 60) / ACCURACY * (size + 5) + px
        y1 = sa(i) / ACCURACY * (size + 5) + py
        
        x2 = sa((i + 15) Mod 60) / ACCURACY * (size + 10) + px
        y2 = sa(i) / ACCURACY * (size + 10) + py
        
       ActiveSheet.Shapes.AddLine x1, y1, x2, y2
    Next
    
    ActiveSheet.Shapes.AddLine x1, y1, x2, y2
End Sub
