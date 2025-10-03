import requests
from bs4 import BeautifulSoup
import time
import urllib.parse
import urllib3

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

def crawl_ecommerce_site(user_url=None):
    if user_url:
        urls = [user_url]
    else:
        urls = [
            "https://scrapeme.live/shop/",
            "http://scrapeme.live/shop/",
            "https://books.toscrape.com/"
        ]   
    
    for url in urls:
        try:
            print(f"Trying to crawl: {url}")
            response = requests.get(url, timeout=15, verify=False)
            response.raise_for_status()
            print(f"Successfully connected to: {url}")
            
            soup = BeautifulSoup(response.text, "html.parser")
            
            if "scrapeme.live" in url:
                product_selectors = [
                    "li.product",
                    "div.product", 
                    "div.product-item",
                    "div[data-product-id]"
                ]
                name_selectors = [".woocommerce-loop-product__title", "h2", "h3", ".product-title"]
                price_selectors = [".price", ".amount", ".woocommerce-Price-amount"]
            elif "books.toscrape.com" in url:
                product_selectors = ["article.product_pod"]
                name_selectors = ["h3 a", "h3"]
                price_selectors = [".price_color"]
            else:
                product_selectors = ["div.product", "li.product", "div.product-item"]
                name_selectors = ["h2", "h3", ".product-title", "a"]
                price_selectors = [".price", ".amount", "span[class*='price']"]
            
            products = []
            for selector in product_selectors:
                products = soup.select(selector)
                if products:
                    print(f"Found {len(products)} products using selector: {selector}")
                    break
            
            if not products:
                print("No products found with standard selectors. Trying alternative approach...")
                product_links = soup.find_all("a", href=True)
                products = [link for link in product_links if any(keyword in link.get('href', '') for keyword in ['/product/', '/item/', '/p/', '/catalogue/'])]
                if products:
                    print(f"Found {len(products)} product links")
            
            count = 0
            for product in products[:10]:
                try:
                    name = None
                    price = None
                    link = None
                    
                    for sel in name_selectors:
                        name_elem = product.select_one(sel)
                        if name_elem:
                            name = name_elem.get_text(strip=True)
                            if name:
                                break
                    
                    for sel in price_selectors:
                        price_elem = product.select_one(sel)
                        if price_elem:
                            price = price_elem.get_text(strip=True)
                            if price:
                                break
                    
                    link_elem = product.find("a", href=True)
                    if link_elem:
                        link = urllib.parse.urljoin(url, link_elem["href"])
                    elif hasattr(product, 'get') and product.get('href'):
                        link = urllib.parse.urljoin(url, product['href'])
                    
                    if name:
                        count += 1
                        print(f"\n--- Product {count} ---")
                        print(f"Name: {name}")
                        print(f"Price: {price if price else 'Not found'}")
                        print(f"Link: {link if link else 'Not found'}")
                        print("-" * 40)
                    
                    time.sleep(0.5)
                    
                except Exception as e:
                    print(f"Error processing product: {e}")
                    continue
            
            if count == 0:
                print("No products could be extracted. The website structure might be different.")
                print("Here's a sample of what the page contains:")
                # Show first few product-like elements
                sample_elements = soup.find_all(['div', 'li', 'article'], limit=5)
                for i, elem in enumerate(sample_elements):
                    print(f"Element {i+1}: {elem.name} - {elem.get('class', 'no class')}")
                    if elem.get_text(strip=True):
                        print(f"  Text preview: {elem.get_text(strip=True)[:100]}...")
                    print()
            else:
                print(f"\nSuccessfully extracted {count} products!")
            
            break
            
        except requests.RequestException as e:
            print(f"Failed to connect to {url}: {e}")
            if url == urls[-1]:
                print("All URLs failed.")
            continue
        except Exception as e:
            print(f"An error occurred while processing {url}: {e}")
            continue


if __name__ == "__main__":
    print("E-commerce Web Crawler")
    print("=" * 30)
    
    print("Options:")
    print("1. Enter custom URL")
    print("2. Use demo sites")
    
    choice = input("\nEnter your choice (1 or 2): ").strip()
    
    if choice == "1":
        user_url = input("Enter the e-commerce website URL: ").strip()
        
        if user_url and not user_url.startswith(('http://', 'https://')):
            user_url = 'https://' + user_url
        
        if user_url:
            print(f"\nCrawling user-provided URL: {user_url}")
            crawl_ecommerce_site(user_url)
        else:
            print("No URL provided. Using demo sites...")
            crawl_ecommerce_site()
    else:
        print("\nUsing demo sites...")
        crawl_ecommerce_site()
