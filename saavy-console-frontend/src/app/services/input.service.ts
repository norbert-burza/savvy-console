import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { OnInit } from '@angular/core';

@Injectable()
export class InputService implements OnInit {

    output:string;

    constructor(private http : HttpClient) {
        console.log('http injected init...');
    }

    ngOnInit():void {
        this.http.get('http://localhost:8080/input').subscribe(data => { this.output = data['output']});
        console.log('service init...');
    }
}

interface ConsoleOutput {
    output:string;
}