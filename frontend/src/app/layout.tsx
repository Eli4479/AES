import Script from 'next/script';
import './globals.css'; // or your global styles
import type { Metadata } from 'next';

export const metadata: Metadata = {
  title: 'AES Encryptor',
  description: 'Secure AES encryption and decryption tool',
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <head>
        <Script
          src="https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4"
          strategy="afterInteractive"
        />
      </head>
      <body>{children}</body>
    </html>
  );
}
