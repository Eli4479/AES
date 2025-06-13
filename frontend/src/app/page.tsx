'use client';

import { useState } from 'react';
import axios from 'axios';

export default function Home() {
    const [input, setInput] = useState('');
    const [key, setKey] = useState('');
    const [mode, setMode] = useState('e');
    const [output, setOutput] = useState('');
    const [loading, setLoading] = useState(false);

    const handleSubmit = async (e: React.FormEvent) => {
        e.preventDefault();
        if (!input || !key || !mode) {
            setOutput('Please provide input, key, and mode.');
            return;
        }
        const trimmedInput = input.trim();
        const trimmedKey = key.trim();
        if (trimmedInput.length === 0 || trimmedKey.length === 0) {
            setOutput('Input and key cannot be empty.');
            return;
        }
        setLoading(true);
        setOutput('');
        try {
            const response = await axios.post('https://aes-srjz.onrender.com/aes', {
                input: trimmedInput,
                key: trimmedKey,
                mode: (mode === 'e' ? 'encrypt' : 'decrypt')
            });
            setOutput(response.data.result)
        } catch (err) {
            console.error('Error fetching data:', err);
            setOutput('Error processing input.');
        } finally {
            setLoading(false);
        }
    };

    return (
        <main style={{
            minHeight: '100vh',
            background: '#18181b',
            display: 'flex',
            flexDirection: 'column',
            alignItems: 'center',
            justifyContent: 'center'
        }}>
            <div style={{
                width: '100%',
                maxWidth: 420,
                background: '#23232a',
                borderRadius: 14,
                padding: 28,
                boxShadow: '0 2px 16px #0006',
                border: '1px solid #27272a'
            }}>
                <h1 style={{
                    fontSize: 26,
                    fontWeight: 700,
                    marginBottom: 20,
                    textAlign: 'center',
                    color: '#fafafa',
                    letterSpacing: 1
                }}>AES Encryption/Decryption</h1>
                <form onSubmit={handleSubmit}>
                    <div style={{ marginBottom: 14 }}>
                        <label style={{ display: 'block', fontWeight: 600, marginBottom: 6, color: '#d4d4d8' }}>Input</label>
                        <textarea
                            rows={4}
                            style={{
                                width: '100%',
                                padding: 10,
                                borderRadius: 6,
                                border: '1px solid #333',
                                fontSize: 15,
                                background: '#18181b',
                                color: '#fafafa',
                                resize: 'vertical'
                            }}
                            value={input}
                            onChange={(e) => setInput(e.target.value)}
                            placeholder="Type your text here..."
                        />
                    </div>
                    <div style={{ marginBottom: 14 }}>
                        <label style={{ display: 'block', fontWeight: 600, marginBottom: 6, color: '#d4d4d8' }}>
                            Key <span style={{ fontSize: 12, color: '#71717a' }}>(max 16 characters)</span>
                        </label>
                        <input
                            style={{
                                width: '100%',
                                padding: 10,
                                borderRadius: 6,
                                border: '1px solid #333',
                                fontSize: 15,
                                background: '#18181b',
                                color: '#fafafa'
                            }}
                            type="text"
                            maxLength={16}
                            value={key}
                            onChange={(e) => setKey(e.target.value)}
                            placeholder="Enter AES key"
                        />
                    </div>
                    <div style={{ marginBottom: 14, display: 'flex', alignItems: 'center' }}>
                        <label style={{ fontWeight: 600, marginRight: 10, color: '#d4d4d8' }}>Mode:</label>
                        <select
                            value={mode}
                            onChange={(e) => setMode(e.target.value)}
                            style={{
                                padding: 8,
                                borderRadius: 6,
                                border: '1px solid #333',
                                fontSize: 15,
                                background: '#18181b',
                                color: '#fafafa'
                            }}
                        >
                            <option value="e">Encrypt</option>
                            <option value="d">Decrypt</option>
                        </select>
                    </div>
                    <button
                        type="submit"
                        style={{
                            width: '100%',
                            background: '#3b82f6',
                            color: '#fff',
                            fontWeight: 700,
                            padding: 12,
                            borderRadius: 6,
                            border: 'none',
                            fontSize: 16,
                            cursor: loading ? 'not-allowed' : 'pointer',
                            marginBottom: 18,
                            transition: 'background 0.2s'
                        }}
                        disabled={loading}
                    >
                        {loading ? 'Loading...' : 'Submit'}
                    </button>
                </form>
                <div>
                    <h2 style={{
                        fontSize: 18,
                        fontWeight: 600,
                        marginBottom: 8,
                        color: '#fafafa'
                    }}>Output:</h2>
                    <div style={{
                        minHeight: 40,
                        padding: 12,
                        background: '#18181b',
                        borderRadius: 6,
                        fontFamily: 'monospace',
                        fontSize: 15,
                        border: '1px solid #27272a',
                        color: '#38bdf8',
                        wordBreak: 'break-all'
                    }}>
                        {loading ? 'Loading...' : (output || 'Type input and key to see output.')}
                    </div>
                </div>
            </div>
        </main>
    );
}
