# PatiOS-Embedded

[![Language: C](https://img.shields.io/badge/Language-C-A8B9CC.svg)](https://ibm.com)
[![AI Assisted](https://img.shields.io/badge/AI_Assisted-orange)](#)

**PatiOS-Embedded**, gömülü sistemler, siber güvenlik altyapıları ve bağımsız mobil platformlar için Linux Çekirdeği (Kernel) mimarisi üzerine **sıfırdan (from scratch)** inşa edilmiş, %100 saf C tabanlı bağımsız bir Linux dağıtım çatısıdır. 

* **Geliştirme Motoru:** Kendi özel dağıtımınızı derlemek ve genişletmek için çevreleyici ekosistem aracı olan **[KedyBox](https://github.com/mehmetdemir-tr/KedyBox)** reposunu kullanabilirsiniz.
* **Kod Adı:** `watermelon-karpuz`

---

## Teknik Mimari ve Öne Çıkan Özellikler

PatiOS-Embedded, Debian/Ubuntu gibi hazır dağıtım tabanlarını kullanmaz. Tamamen ham çekirdek üzerine inşa edilen kök dosya sistemi (rootfs) ve kullanıcı alanı (user-space) araçlarından oluşur:

* **Sıfır Bağımlılık (Standalone User-Space):** Sistem, harici ağır paket yığınlarına veya yorumlayıcılara (Python vb.) ihtiyaç duymadan doğrudan saf C ikilileri (binary) ile çalışır.
* **Karabaş Servisi (`pati-services`):** Arka planda kritik sistem süreçlerini, bellek sızıntılarını ve proses durumlarını mikroskobik düzeyde izleyen, tamamen C ile yazılmış yerli proses takip (init/daemon) mekanizmasıdır.
* **Gelişmiş İzole Kabuk (`shell.c`):** Üçüncü parti kabuk bileşenlerine bağımlı kalmadan, sistemin en alt katmanıyla doğrudan donanım seviyesinde haberleşen, hafif ve güvenli komut satırı arayüzüdür.
* **Musl-libc Optimizasyonu:** `aarch64-linux-musl-gcc` zinciri hedeflenerek derlenmiştir. Bu sayede standart `glibc` kütüphanelerine kıyasla bellek taşması (buffer overflow) gibi siber güvenlik zafiyetlerine karşı doğal koruma ve ultra hafif binary boyutu sağlar.

---

## Proje İçeriği ve Dosya Yapısı

* `shell.c` : İşletim sistemi ana kabuğu, kullanıcı etkileşimi ve alt komut yönetim mimarisi.
* `mauvyd.c` : Çekirdek başlangıcından sonra devreye giren temel sistem yönetim dosyası.
* `pati-services/` : `karabaş` gibi kritik arka plan servislerinin ve sistem daemonlarının kaynak kodları.
* `Kernel Yapılandırması` : ARM64 ve Raspberry Pi mimarileri için optimize edilmiş minimal, sertleştirilmiş çekirdek dosyaları.

---

## 🚀 Dağıtım Kurulumu ve Hedef Platformlar

### Desteklenen Donanımlar
* Raspberry Pi 3 / 4 / 5 (ARM64)
* QEMU (Sanallaştırma ortamları)
* WSL (Windows Subsystem for Linux) ve Yerel Linux Dağıtımları

### Canlı Kurulum Protokolü (Raspberry Pi 3/4/5)
1. SD kartınızı **MBR** düzeniyle bölümlendirin ve minimum **512MB FAT32** alanı oluşturun.
2. [Raspberry Pi Firmware](https://github.com) reposundan `boot` klasör içeriğini bu bölüme taşıyın.
3. KedyBox ile derlediğiniz `initramfs` dosyasının adını `initramfs.gz` yaparak FAT32 bölümüne yükleyin.
4. `cmdline.txt` dosyasını oluşturup şu parametreleri ekleyin:  
   `console=serial0,115200 console=tty1 rdinit=/init`
5. `config.txt` dosyasını oluşturup sistemi şu optimize parametrelerle yapılandırın:
   ```text
   display_auto_detect=1
   initramfs initramfs.gz followkernel
   disable_fw_kms_setup=1
   arm_64bit=1
   arm_boost=1
   ```

---

## Sistem Ekran Görüntüleri

![Pati Shell](https://raw.githubusercontent.com/mehmetdemir-tr/Pati/main/screenshots/genel.jpeg)  
*Pati Shell — İşletim sisteminin bağımsız, saf C ile yazılmış ana kabuk arayüzü.*

![Yardım](https://raw.githubusercontent.com/mehmetdemir-tr/Pati/main/screenshots/yardim.jpeg)  
*`yardim` komutu — Donanıma doğrudan erişen tüm yerleşik sistem komutları.*

![Patifetch](https://raw.githubusercontent.com/mehmetdemir-tr/Pati/main/screenshots/patifetch.jpeg)  
*`patifetch` — Çekirdek seviyesinden dinamik veri çeken sistem bilgi aracı.*

---

## Lisans ve Katkıda Bulunma

Bu proje **MIT** lisansı ile yayınlanmaktadır. 
* Projenin tüm telif hakları açık kalmak kaydıyla, ticari ve kurumsal projelerde kapatılarak veya entegre edilerek kullanılması tamamen serbesttir.
* Projeye katkı sağlamak, hata bildirmek veya siber güvenlik yaması eklemek için lütfen **Issues** sekmesi üzerinden yeni bir konu açın.

**Geliştirme Notu:** Bu proje, araştırma ve derin mühendislik odaklı bir protokolle geliştirilmektedir. Yapay zeka yardımı alınırken doğrudan kod kopyalamak yerine, işletim sistemi teorisi, terim araştırması ve alt seviye mantık sorgulama yöntemi tercih edilmektedir.

---
*Geliştirici: [mehmetdemir-tr](https://github.com/mehmetdemir-tr)*
