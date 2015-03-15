Planner, robotun yapacağı ana görevi XML formatlı bir plan dosyasından okuyarak, gerekli davranışları aktif hale getirmekten ve sonraki adımlarda davranışların açılıp kapatılmasından sorumlu katmandır. Bu katman sayesinde robotun yapacağı iş değişse bile, program tasarımını değiştirmeye gerek kalmadan, sadece bir XML dosyasını düzenleyerek amaca ulaşmak mümkün olur.

Plan dosyası, robotun hareketlerinin hangi adımlardan oluşacağını, her bir adım için hangi davranışların aktif halde bulunması gerektiğini tanımlar. Davranışlardan gelecek sinyallere göre de hangi adımlara geçileceği bu dosyada listelenir.

Planner'ın işleyişini daha iyi görmek için örnek bir senaryo ve buna ait XML dosyasını görelim:

ODTÜ Müzesi, çizgi ile işaretlenmiş bir yoldan ve yol üzerinde bulunan çeşitli eserler için de ayrıca işaretlerden oluşsun. Robotumuz, müzenin girişinde beklesin. İnsanlar girdiğinde karşılayıp, çizgi izlemeye başlayarak her gördüğü eseri insanlara açıklasın ve yoluna devam ederek yolun sonuna ulaştığında ziyaretçileri uğurlasın. Bu senaryoyu açılayan plan dosyası aşağıdaki gibidir:

```
<plan start="Welcome">
    <states>

        <state name="Welcome">
            <behaviours>
                <behaviour name="Voice" arguments="welcome.wav"/>
            </behaviours>
            <transitions>
                <transition expression="Voice=finished" nextstate="FindMarker" />
            </transitions>
        </state>
        
        <state name="FindMarker">
            <behaviours>
                <behaviour name="LineTracking" />
                <behaviour name="ObstacleAvoidance" />
                <behaviour name="DetectMarker" />
            </behaviours>
            <transitions>
                <transition expression="DetectMarker=standartmarker" nextstate="Explain"/>
                <transition expression="DetectMarker=endmarker" nextstate="Finish" />
            </transitions>
        </state>
        
        <state name="Explain">
            <behaviours>
                <behaviour name="Voice" arguments="explain.wav"/>
		<behaviour name="Stop" />
            </behaviours>
            <transitions>
                <transition expression="Voice=finished" nextstate="FindMarker" />
            </transitions>
        </state>
    
        <state name="Finish">
            <behaviours>
                <behaviour name="Voice" arguments="bye.wav"/>
		<behaviour name="Stop" />
            </behaviours>
            <transitions>
            </transitions>
        </state>
    </states>
</plan>
```

