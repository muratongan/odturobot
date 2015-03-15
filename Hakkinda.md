# Giriş #
Bu projede elimizdeki Pioneer 3 DX model bir robot tabanını programlayarak, çeşitli alanlarda kullanılabilecek akıllı bir robot oluşturmayı amaçlıyoruz. Bu belge, robot yazılımının tasarımı hakkında bilgi vermek için yazılmıştır.
# Yazılım Katmanları #
![http://www.cclub.metu.edu.tr/robot/tasarim.png](http://www.cclub.metu.edu.tr/robot/tasarim.png)

Robotla seri port vasıtasıyla iletişim kuran ve temel fonksiyonların yerine getirilmesini sağlayan katman, player'dır. Bunun üstünde, robot tehlikeli bir durumdayken (duvara çok yakınken) onu durdurmaya yarayan ve üstteki parçalarla iletişimi sağlayan watchdog yer alır. Çeşitli davranışlardan aldığı farklı komutlar arasında karar verip watchdog'a ileten ise arbitrator (hakem) katmanıdır. Arbitrator, sadece belli, küçük bir işi yapmak üzere hazırlanmış behaviour denen davranışlara bağlıdır. Bunlar da robotun yapacağı ana görevi bilen ve davranışları ona göre yönlendiren planner katmanına bağlıdır. Planner katmanı, robotun uygulaması gereken senaryoyu özel formatlı bir xml dosyasından alır.

Robot farklı araçları da kullanarak davranışlarını yönlendirebilir. Bunlar, kamera, hoparlör, mikrofon, kafa, gövde gibi yardımcı nesneler olabilir. Bu araçlar, arbitrator katmanına bağlanır ve behaviour'ların bunlara erişimi yine arbitrator aracılığı ile sağlanır.

Bu modelde, farklı katmanlar farklı işlerden sorumludur. Bu yapı sayesinde, her bir parça sadece kendi yapacağı işle ilgilenir ve modülerlik sağlanır. Ayrıca, robotun yapacağı göreve göre diğer katmanlara dokunmadan yalnızca plan.xml dosyası değiştirilerek, robotun istenen herhangi bir işlevi gerçekleştirmesi sağlanabilir.