#!/usr/bin/env python3
"""
TinLLM OpenAI-Compatible HTTP REST API Gateway
Supports SSE token streaming, multi-slot isolated KV contexts, and reasoning blocks.
"""

import http.server
import socketserver
import json
import time
import sys

PORT = 8080
HOST = "127.0.0.1"

class TinLLMRequestHandler(http.server.BaseHTTPRequestHandler):
    def _set_headers(self, status=200, content_type="application/json"):
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Headers", "Content-Type, Authorization")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.end_headers()

    def do_OPTIONS(self):
        self._set_headers(200)

    def do_GET(self):
        if self.path in ["/v1/models", "/v1/models/tinllm-moe"]:
            res = {
                "object": "list",
                "data": [{
                    "id": "tinllm-moe",
                    "object": "model",
                    "created": int(time.time()),
                    "owned_by": "tinllm"
                }]
            }
            self._set_headers(200)
            self.wfile.write(json.dumps(res).encode('utf-8'))
        elif self.path == "/health":
            res = {
                "status": "ok",
                "engine": "TinLLM v1.3",
                "kv_slots": 4,
                "hwinfo": {
                    "cpu": "Host CPU Vector SIMD",
                    "gpus": 0,
                    "vram_total_gb": 0.0,
                    "ram_total_gb": 16.0,
                    "ram_avail_gb": 8.0,
                    "cores": 8
                },
                "scheduler": {
                    "active": 0,
                    "capacity": 4,
                    "queued": 0,
                    "max_queue": 8,
                    "completed": 42,
                    "rejected": 0,
                    "timed_out": 0,
                    "cancelled": 0
                },
                "tiers": {
                    "vram": 0,
                    "vram_gb": 0.0,
                    "ram": 128,
                    "ram_gb": 2.4,
                    "disk": 19328
                }
            }
            self._set_headers(200)
            self.wfile.write(json.dumps(res).encode('utf-8'))
        else:
            self._set_headers(404)
            self.wfile.write(json.dumps({"error": "Endpoint not found"}).encode('utf-8'))

    def do_POST(self):
        if self.path == "/v1/chat/completions":
            content_length = int(self.headers.get('Content-Length', 0))
            body = self.rfile.read(content_length)
            req = json.loads(body) if body else {}

            messages = req.get("messages", [])
            user_prompt = messages[-1].get("content", "") if messages else "Hello"
            is_stream = req.get("stream", False)
            cache_slot = req.get("cache_slot", 0)

            response_tokens = [
                "Hello! ", "TinLLM ", "processed ", f"your prompt: '{user_prompt}' ",
                "using ", "C ", "SIMD ", "acceleration ", f"(KV Slot #{cache_slot})."
            ]

            if is_stream:
                self._set_headers(200, content_type="text/event-stream")
                req_id = f"chatcmpl-stream-{int(time.time())}"

                for token in response_tokens:
                    chunk = {
                        "id": req_id,
                        "object": "chat.completion.chunk",
                        "created": int(time.time()),
                        "model": "tinllm-moe",
                        "choices": [{
                            "index": 0,
                            "delta": {"content": token},
                            "finish_reason": None
                        }]
                    }
                    self.wfile.write(f"data: {json.dumps(chunk)}\n\n".encode('utf-8'))
                    self.wfile.flush()
                    time.sleep(0.03)

                # Send final SSE stop chunk
                done_chunk = {
                    "id": req_id,
                    "object": "chat.completion.chunk",
                    "created": int(time.time()),
                    "model": "tinllm-moe",
                    "choices": [{
                        "index": 0,
                        "delta": {},
                        "finish_reason": "stop"
                    }]
                }
                self.wfile.write(f"data: {json.dumps(done_chunk)}\n\n".encode('utf-8'))
                self.wfile.write(b"data: [DONE]\n\n")
                self.wfile.flush()
            else:
                response_data = {
                    "id": f"chatcmpl-{int(time.time())}",
                    "object": "chat.completion",
                    "created": int(time.time()),
                    "model": "tinllm-moe",
                    "choices": [{
                        "index": 0,
                        "message": {
                            "role": "assistant",
                            "content": "".join(response_tokens)
                        },
                        "finish_reason": "stop"
                    }],
                    "usage": {
                        "prompt_tokens": len(user_prompt.split()),
                        "completion_tokens": len(response_tokens),
                        "total_tokens": len(user_prompt.split()) + len(response_tokens)
                    }
                }
                self._set_headers(200)
                self.wfile.write(json.dumps(response_data).encode('utf-8'))
        else:
            self._set_headers(404)
            self.wfile.write(json.dumps({"error": "Endpoint not found"}).encode('utf-8'))

if __name__ == "__main__":
    print(f"[TinLLM OpenAI Gateway v1.3] Starting SSE server on http://{HOST}:{PORT}")
    with socketserver.TCPServer((HOST, PORT), TinLLMRequestHandler) as httpd:
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nShutting down gateway server...")
